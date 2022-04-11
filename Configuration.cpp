#include "Configuration.hpp"
#include <arpa/inet.h>
#include "utils.hpp"


Component::Component (const std::string& dir, bool sin): _dir (dir), _sin (sin) {}
Component::~Component () {}

SimpleComponent::SimpleComponent (const std::string& _dir, bool _sin): Component (_dir, _sin) {}
SimpleComponent::~SimpleComponent () {}
void SimpleComponent::pretty_print (std::string _tab) const {
	if (isSet ()) {
		std::cout << _tab << _dir << " ";
		print (_tab + "\t");
		std::cout << ";\n";
	}
}
void SimpleComponent::syntax_parse (Tokenizer &tokenizer) {
	if (_sin && isSet ()) throw Error (tokenizer.error ("multiple directive, same name"));
	tokenizer.expect (_dir);
	parse (tokenizer);
	if (!isSet ()) throw Error (tokenizer.error ("empty directive"));
	tokenizer.expect (";");
}

BracketedComponent::BracketedComponent (const std::string& _dir, bool _sin): Component (_dir, _sin) {}
BracketedComponent::~BracketedComponent () {}
void BracketedComponent::pretty_print (std::string _tab) const {
	if (isSet ()) {
		std::cout << _tab << _dir << " {\n";
		print (_tab + "\t");
		std::cout << _tab << "}\n";
	}
}
void BracketedComponent::syntax_parse (Tokenizer& tokenizer) {
	if (_sin && isSet ()) throw Error (tokenizer.error ("multiple directive, same name"));
	tokenizer.expect (_dir);
	tokenizer.expect ("{");
	parse (tokenizer);
	tokenizer.expect ("}");
	if (_dir == "http" && tokenizer)
		throw Error (tokenizer.error ("directive not allowed"));
}

SuffixBracketedComponent::SuffixBracketedComponent (const std::string& _dir, bool _sin): Component (_dir, _sin) {}
SuffixBracketedComponent::~SuffixBracketedComponent () {}
std::string& SuffixBracketedComponent::_suffix () { return _suf; }
void SuffixBracketedComponent::pretty_print (std::string _tab) const {
	if (isSet ()) {
		std::cout << _tab << _dir << " " << _suf << " {\n";
		print (_tab + "\t");
		std::cout << _tab << "}\n";
	}
}
void SuffixBracketedComponent::syntax_parse (Tokenizer& tokenizer) {
	if (_sin && isSet ()) throw Error (tokenizer.error ("multiple directive, same name"));
	tokenizer.expect (_dir);
	_suf = (*tokenizer).id ();
	++tokenizer;
	tokenizer.expect ("{");
	parse (tokenizer);
	tokenizer.expect ("}");
}

// must respect network byte order
Address::Address (): is_set (false), address (0) {}
Address::~Address () {}
u_int32_t Address::getAdress () const { return address; }
void Address::setAddress (u_int32_t addr) { address = addr; }
const char *Address::print () const {
	struct in_addr addr;
	addr.s_addr = address;
	return inet_ntoa (addr);
}
void Address::parse (Tokenizer& tokenizer) {
	std::string address_token = tokenizer.id ();
	if (address_token.find ('.') != std::string::npos){
		struct in_addr addr;
		if (inet_pton (AF_INET, address_token.c_str (), &addr.s_addr) == 1) {
			is_set = true;
			address = addr.s_addr;
			++tokenizer;
			return;
		}
		throw Error (tokenizer.error("expected address"));
	}
}

Port::Port ():  is_set (false), port (0) {}
Port::~Port () {}
u_int16_t Port::getPort () const { return port; }
void Port::setPort (u_int16_t prt) { port = prt; }
void Port::parse (Tokenizer& tokenizer) {
	std::string address = tokenizer.id ();
	if (!(*tokenizer).is_directive ()) {
		if (all_of_ (address.begin (), address.end (), is_digit ())) {
			unsigned int prt = std::atoi (address.c_str ());
			if (prt <= 0xFFFF) {
				++tokenizer;
				port = prt;
				is_set  = true;
			}
		}
		else
			throw Error (tokenizer.error ("wrong port"));
	}
}

Listen::Listen (): SimpleComponent ("listen") {}
Listen::~Listen () {}
bool Listen::isSet () const { return _prt.isSet () || _addr.isSet (); }
void Listen::parse (Tokenizer& tokenizer) {
	_addr.parse (tokenizer);
	_prt.parse (tokenizer);
}
Listen* Listen::clone () {
	Listen *_listen = new Listen ();
	*_listen = *this;
	return _listen; 
}

void Listen::print (std::string tabulation) const {
	std::cout << _addr.print() << ":" << _prt.getPort ();
}


Root::Root (): SimpleComponent ("root") {}
Root::~Root () {}
bool Root::isSet () const { return !root.empty (); }
Root* Root::clone () { return new Root(*this); }
void Root::parse (Tokenizer &tokenizer) {
	root = (*tokenizer).id ();
	++tokenizer;
}
void Root::print (std::string tabulation) const {
	std::cout << root;
}


Index::Index (): SimpleComponent ("index") {}
Index::~Index () {}
bool Index::isSet () const { return !indexes.empty (); }
Index* Index::clone () { return new Index (*this); }
void Index::parse (Tokenizer & tokenizer) {
	while (!(*(tokenizer)).is_directive ()) {
		indexes.push_back ((*tokenizer).id ());
		++tokenizer;
	}
}
void Index::print (std::string tabulation) const {
	for (std::vector<std::string>::const_iterator it = indexes.begin (); it != indexes.cend(); ++it) {
		std::cout << *it << " ";
	}
}

AutoIndex::AutoIndex (): SimpleComponent ("auto_index"), on ("") {}
AutoIndex::~AutoIndex () {}
AutoIndex* AutoIndex::clone () { return new AutoIndex (*this); }
bool AutoIndex::isSet () const { return !on.empty (); }
void AutoIndex::parse (Tokenizer& tokenizer) {
	if (tokenizer.id () == "on" || tokenizer.id () == "off") {
		on = (*tokenizer).id ();
		++tokenizer;
	}
	else
		throw Error (tokenizer.error ("expected on/off"));
}
void AutoIndex::print (std::string tabulation) const {
	std::cout << on;
}

BodySize::BodySize (): SimpleComponent ("max_body_size"), size (0), already_set (false) {}
BodySize::~BodySize () {}
void BodySize::setBodySize (unsigned int sz) { size = sz; } 
unsigned int BodySize::getBodySize () const { return size; }
BodySize* BodySize::clone () { return new BodySize (*this); }
bool BodySize::isSet () const { return already_set; }
void BodySize::parse (Tokenizer& tokenizer) {
	std::string sz = tokenizer.id ();
	if (sz.back() == 'M' && all_of_ (sz.begin (), sz.end () - 1, is_digit ())) {
		already_set = true;
		size = atoi (sz.c_str ()) * 1024;
		++tokenizer;
	}
	else
		throw Error (tokenizer.error ("max body size format XXXM"));
}
void BodySize::print (std::string tabulation) const {
	std::cout << size;
}

AllowedMethods::AllowedMethods (): SimpleComponent ("allowed_methods") {}
AllowedMethods::~AllowedMethods () {}
bool AllowedMethods::isSet () const { return !methods.empty (); }
AllowedMethods* AllowedMethods::clone () { return new AllowedMethods (*this); }
void AllowedMethods::parse (Tokenizer& tokenizer) {
	std::pair<std::set<std::string>::iterator,bool> ret;
	while (!(*tokenizer).is_directive ()) {
		ret = methods.insert (tokenizer.id ());
		if (ret.second == false)
			throw Error (tokenizer.error ("multiple methods with same name"));
		++tokenizer;
	}
}
void AllowedMethods::print (std::string tabulation) const {
	for (std::set <std::string>::iterator it = methods.begin (); it != methods.end (); ++it) {
		std::cout << *it << " ";
	}
}

ErrorPages::ErrorPages (): SimpleComponent ("error_page", false) {}
ErrorPages::~ErrorPages () {}
ErrorPages* ErrorPages::clone () { return new ErrorPages (*this); }
bool ErrorPages::isSet () const { return !error_pages.empty (); }
void ErrorPages::parse (Tokenizer& tokenizer) {
	unsigned int code;
	std::string		path;
	if ((*tokenizer).isNumber ()) {
		code = atoi (tokenizer.id ().c_str ());
		if (code <= 0xFFFF) {
			++tokenizer;
			if (!(*tokenizer).is_directive ()) {
				path = tokenizer.id ();
				if (error_pages.insert (std::make_pair (code, path)).second == false)
					throw Error (tokenizer.error ("error page already set"));
				++tokenizer;
				return;
			}
			throw Error (tokenizer.error ("expected path"));
		}
		throw Error (tokenizer.error ("code out of range"));
	}
	throw Error (tokenizer.error ("expected a code"));
}
void ErrorPages::print (std::string tabulation) const {
	for (std::map <unsigned short, std::string>::const_iterator it = error_pages.cbegin (); it != error_pages.end (); ++it) {
		std::cout << it->first << ":" << it->second << " ";
	}
}

ServerNames::ServerNames (): SimpleComponent ("server_name") {}
ServerNames::~ServerNames () {}
ServerNames* ServerNames::clone () { return new ServerNames (*this); }
bool ServerNames::isSet () const { return !names.empty (); }
void ServerNames::parse (Tokenizer& tokenizer) {
	std::pair<std::set<std::string>::iterator,bool> ret;
	while (tokenizer && !(*(tokenizer)).is_directive ()) {
		ret = names.insert (tokenizer.id ());
		if (ret.second == false)
			throw Error (tokenizer.error ("Server Names with same name"));
		++tokenizer;
	}
}
void ServerNames::print (std::string tabulation) const {
	for (std::set <std::string>::iterator it = names.begin (); it != names.end (); ++it) {
		std::cout << *it << " ";
	}
}

Location::Location (): SuffixBracketedComponent ("location") {
	_com.push_back (new Index ());
	_com.push_back (new Root ());
	_com.push_back (new AutoIndex ());
	_com.push_back (new AllowedMethods ());
}
Location::~Location () {
	for (std::vector <Component *>::iterator it = _com.begin (); it != _com.end (); ++it)
		delete (*it);
}
Location& Location::operator =(const Location& rhs) {
	if (this != &rhs) {
		for (std::vector <Component *>::iterator it = _com.begin (); it != _com.end (); ++it)
			delete (*it);
		_com.clear ();
		for (std::vector <Component *>::const_iterator it = rhs._com.cbegin (); it != rhs._com.cend (); ++it)
			_com.push_back ((*it)->clone ());
	}
	return *this;
}
Location* Location::clone () { return new Location (*this); }
std::string Location::path () const { return _suf; }
bool Location::isSet () const {
	for (std::vector <Component *>::const_iterator it = _com.begin (); it != _com.end (); ++it)
		if ((*it)->isSet ()) return true;
	return false;
}
void Location::parse (Tokenizer& tokenizer) {
	while (tokenizer && tokenizer.id () != "}") {
		bool set = false;
		for (std::vector <Component *>::iterator it = _com.begin (); it != _com.end (); ++it) {
			if ((*it)->dir() == tokenizer.id ()) {
				(*it)->syntax_parse (tokenizer);
				set = true;
				break;
			}
		}
		if (!set)
			throw Error (tokenizer.error ("directive not allowed"));
	}
}
void Location::print (std::string tabulation) const {
	for (std::vector <Component *>::const_iterator it = _com.cbegin (); it != _com.cend (); ++it)
		(*it)->pretty_print (tabulation + "\t");
}

Locations::Locations () {}
Locations::~Locations () {}
bool Locations::isSet () const { return !locations.empty (); }
void Locations::parse (Tokenizer& tokenizer) {
	Location location;
	location.syntax_parse (tokenizer);
	if (location.isSet ())
		locations.push_back (location);
}
void Locations::print (std::string tabulation) const {
	for (std::vector<Location>::const_iterator it = locations.cbegin (); it != locations.end (); ++it) {
		it->pretty_print (tabulation);
	}
}

Server::Server (): BracketedComponent ("server", false) {
	_com.push_back (new Index ());
	_com.push_back (new Root ());
	_com.push_back (new Listen ());
	_com.push_back (new ErrorPages ());
	_com.push_back (new ServerNames ());
	_com.push_back (new BodySize ());
}
Server& Server::operator= (const Server& rhs) {
	if (this != &rhs) {
		locations = rhs.locations;
		for (std::vector<Component*>::iterator it = _com.begin (); it != _com.end (); ++it)
			delete *it;
		_com.clear ();
		for (std::vector<Component*>::const_iterator it = rhs._com.cbegin (); it != rhs._com.cend (); ++it)
			_com.push_back ((*it)->clone ());
	}
	return *this;
} 
Server* Server::clone () {return new Server (*this); }
Server::~Server () {
	for (std::vector <Component *>::iterator it = _com.begin (); it != _com.end (); ++it) {
		delete (*it);
	}
}
bool Server::isSet () const {
	for (std::vector <Component *>::const_iterator it = _com.cbegin (); it != _com.cend (); ++it) {
		if ((*it)->isSet ())
			return true;
	}
	return (locations.isSet ());
}
void Server::parse (Tokenizer& tokenizer) {
	while (tokenizer && tokenizer.id () != "}") {
		bool set = false;
		for (std::vector <Component *>::iterator it = _com.begin (); it != _com.end (); ++it) {
			if ((*it)->dir() == tokenizer.id ()) {
				(*it)->syntax_parse (tokenizer);
				set = true;
				break;
			}
		}
		if (tokenizer.id () == "location") {
			locations.parse (tokenizer);
			set = true;
		}
		if (!set)
			throw Error (tokenizer.error ("directive not allowed"));	
	}
}
void Server::print (std::string tabulation) const {
	for (std::vector <Component *>::const_iterator it = _com.cbegin (); it != _com.cend (); ++it)
		(*it)->pretty_print (tabulation + "\t");
	locations.print (tabulation);
}

vServers::vServers () {}
vServers::~vServers () {}
bool vServers::isSet () const { return !servers.empty (); }
void vServers::parse (Tokenizer& tokenizer){
	Server server;
	server.syntax_parse (tokenizer);
	if (server.isSet ())
		servers.push_back (server);
}
void vServers::print (std::string tabulation) const {
	for (std::vector<Server>::const_iterator it = servers.cbegin (); it != servers.cend (); ++it) {
		it->pretty_print (tabulation);
	}
}


HttpConfig::HttpConfig (): BracketedComponent ("http", true) {}
HttpConfig::~HttpConfig () {}
HttpConfig* HttpConfig::clone () { return new HttpConfig (*this); }
bool HttpConfig::isSet () const { return index.isSet () || root.isSet () || servers.isSet (); }
void HttpConfig::parse (Tokenizer& tokenizer) {
	while (tokenizer && tokenizer.id () != "}") {
		const std::string &direct = (*tokenizer).id ();
		if (direct == "root")
			root.syntax_parse (tokenizer);
		else if (direct == "index")
			index.syntax_parse (tokenizer);
		else if (direct == "server")
			servers.parse (tokenizer);
		else
			throw Error (tokenizer.error ("directive not allowed"));
	}
}
void HttpConfig::print (std::string tabulation) const {
	index.pretty_print (tabulation);
	root.pretty_print (tabulation);
	servers.print (tabulation);
}