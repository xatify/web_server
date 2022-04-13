#include "Configuration.hpp"
#include <arpa/inet.h>
#include "utils.hpp"

std::map <std::string, COMP> NameOfComponent () {
	std::map <std::string, COMP> mp;
	mp.insert (std::make_pair ("listen", LISTEN));
	mp.insert (std::make_pair ("root", ROOT));
	mp.insert (std::make_pair ("index", INDEX));
	mp.insert (std::make_pair ("auto_index", AUTOINDEX));
	mp.insert (std::make_pair ("max_body_size", BODYSIZE));
	mp.insert (std::make_pair ("allowed_methods", ALLOWEDMETHODS));
	mp.insert (std::make_pair ("error_page", ERRORPAGES));
	mp.insert (std::make_pair ("server_name", SERVERNAMES));
	mp.insert (std::make_pair ("location", LOCATION));
	mp.insert (std::make_pair ("server", SERVER));
	mp.insert (std::make_pair ("http", HTTPCONFIG));
	mp.insert (std::make_pair ("upload", UPLOAD));
	return mp;
}

std::map <std::string, COMP> ComponentCreator::mp = NameOfComponent ();

Component::Component (const std::string& dir, bool sin, unsigned _sub):  _sin (sin), _dir (dir), _subs (_sub) {}
Component::Component (const Component& _rhs): _sin (_rhs._sin), _dir (_rhs._dir), _subComp (_rhs._subComp){}
Component& Component::operator = (const Component& _rhs) {
	if (this != &_rhs) {
		_sin = _rhs._sin;
		_dir = _rhs._dir;
		_subComp = _rhs._subComp;
	}
	return *this;
}
Component::~Component () {
	for (std::multimap <std::string, Component *>::iterator it = _subComp.begin (); it != _subComp.end (); ++it)
		delete ((*it).second);
}

bool Component::isSet () const {
	return !_subComp.empty ();
}

bool Component::isSub (const std::string& dir) const {
	return (_subs & ComponentCreator::instance ().id (dir));
}
SimpleComponent::SimpleComponent (const std::string& _dir, bool sin): Component (_dir, sin) {}
SimpleComponent::SimpleComponent (const SimpleComponent& _rhs): Component (_rhs) {}
SimpleComponent& SimpleComponent::operator = (const SimpleComponent& _rhs) {
	if (this != &_rhs) 
		Component::operator= (_rhs);
	return *this;
}

SimpleComponent::~SimpleComponent () {}
void SimpleComponent::pretty_print (std::string _tab) const {
	if (isSet ()) {
		std::cout << _tab << _dir << " ";
		print (_tab + "\t");
		std::cout << ";\n";
	}
}
void SimpleComponent::syntax_parse (Tokenizer &tokenizer) {
	tokenizer.expect (_dir);
	parse (tokenizer);
	if (!isSet ()) throw Error (tokenizer.error ("empty directive"));
	tokenizer.expect (";");
}

BracketedComponent::BracketedComponent (const std::string& _dir, bool sin, unsigned _sub): Component (_dir, sin, _sub) {}
BracketedComponent::BracketedComponent (const BracketedComponent& _rhs): Component (_rhs) {}
BracketedComponent& BracketedComponent::operator = (const BracketedComponent& _rhs) {
	if (this != &_rhs)
		Component::operator= (_rhs);
	return *this;
}
BracketedComponent::~BracketedComponent () {}
void BracketedComponent::pretty_print (std::string _tab) const {
	if (isSet ()) {
		std::cout << _tab << _dir << " {\n";
		for (std::map<std::string, Component *>::const_iterator it = _subComp.begin (); it != _subComp.end (); ++it)
			(*it).second->pretty_print (_tab + "\t");
		std::cout << _tab << "}\n";
	}
}
void BracketedComponent::syntax_parse (Tokenizer& tokenizer) {
	tokenizer.expect (_dir);
	tokenizer.expect ("{");
	while (tokenizer && (*tokenizer).id () != "}") {
		std::string id = tokenizer.id ();
		if (isSub (id)) {
			std::multimap <std::string, Component *>::iterator it = _subComp.find (id);
			if (it != _subComp.end ()) {
				if (((*it).second)->sin ())
					throw Error (tokenizer.error ("only one component allowed"));
			}
			Component * comp = ComponentCreator::instance ().create (id);
			comp->syntax_parse (tokenizer);
			_subComp.insert (std::make_pair (id, comp));
		}
		else
			throw Error (tokenizer.error ("not a sub component"));
	}
	tokenizer.expect ("}");
}

SuffixBracketedComponent::SuffixBracketedComponent (const std::string& _dir, bool sin, unsigned _sub): Component (_dir, sin, _sub) {}
SuffixBracketedComponent::SuffixBracketedComponent (const SuffixBracketedComponent& _rhs): Component (_rhs), _suf (_rhs._suf) {}
SuffixBracketedComponent& SuffixBracketedComponent::operator = (const SuffixBracketedComponent& _rhs) {
	if (this != &_rhs) {
		Component::operator= (_rhs);
		_suf = _rhs._suf;
	}
	return *this;
}
SuffixBracketedComponent::~SuffixBracketedComponent () {}
std::string& SuffixBracketedComponent::_suffix () { return _suf; }
void SuffixBracketedComponent::pretty_print (std::string _tab) const {
	if (isSet ()) {
		std::cout << _tab << _dir << " " << _suf << " {\n";
		for (std::map<std::string, Component *>::const_iterator it = _subComp.begin (); it != _subComp.end (); ++it)
			(*it).second->pretty_print (_tab + "\t");
		std::cout << _tab << "}\n";
	}
}
void SuffixBracketedComponent::syntax_parse (Tokenizer& tokenizer) {
	tokenizer.expect (_dir);
	_suf = (*tokenizer).id ();
	++tokenizer;
	tokenizer.expect ("{");
	while (tokenizer && (*tokenizer).id () != "}") {
		std::string id = tokenizer.id ();
		if (isSub (id)) {
			std::multimap <std::string, Component *>::iterator it = _subComp.find (id);
			if (it != _subComp.end ()) {
				if (((*it).second)->sin ())
					throw Error (tokenizer.error ("only one component allowed"));
			}
			Component * comp = ComponentCreator::instance ().create (id);
			comp->syntax_parse (tokenizer);
			_subComp.insert (std::make_pair (id, comp));
		}
		else
			throw Error (tokenizer.error ("not a sub component"));
	}
	tokenizer.expect ("}");
}

// must respect network byte order
Address::Address (): is_set (false), address (0) {}
Address::Address (const Address& _rhs): is_set (_rhs.is_set), address (_rhs.is_set) {}
Address& Address::operator= (const Address& _rhs) {
	if (this != &_rhs) {
		is_set = _rhs.is_set;
		address = _rhs.address;
	}
	return *this;
}
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
Port::Port (const Port& _rhs): is_set (_rhs.is_set), port (_rhs.is_set) {}
Port& Port::operator= (const Port& _rhs) {
	if (this != &_rhs) {
		is_set = _rhs.is_set;
		port = _rhs.port;
	}
	return *this;
}
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
Listen::Listen (const Listen& _rhs): SimpleComponent (_rhs), _addr (_rhs._addr), _prt (_rhs._prt) {}
Listen& Listen::operator= (const Listen& _rhs) {
	if (this != &_rhs) {
		_addr = _rhs._addr;
		_prt = _rhs._prt;
	}
	return *this;
}
Listen::~Listen () {}
bool Listen::isSet () const { return _prt.isSet () || _addr.isSet (); }
void Listen::parse (Tokenizer& tokenizer) {
	_addr.parse (tokenizer);
	_prt.parse (tokenizer);
}
Listen* Listen::clone () { return new Listen (*this); }

void Listen::print (std::string tabulation) const {
	std::cout << _addr.print() << ":" << _prt.getPort ();
}


Root::Root (): SimpleComponent ("root") {}
Root::Root (const Root& _rhs): SimpleComponent (_rhs), root (_rhs.root) {}
Root& Root::operator= (const Root& _rhs) {
	if (this != &_rhs)
		root = _rhs.root;
	return *this;
}
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

Upload::Upload (): SimpleComponent ("upload") {}
Upload::Upload (const Upload& _rhs): SimpleComponent (_rhs), root (_rhs.root) {}
Upload& Upload::operator= (const Upload& _rhs) {
	if (this != &_rhs)
		root = _rhs.root;
	return *this;
}
Upload::~Upload () {}
bool Upload::isSet () const { return !root.empty (); }
Upload* Upload::clone () { return new Upload(*this); }
void Upload::parse (Tokenizer &tokenizer) {
	root = (*tokenizer).id ();
	++tokenizer;
}
void Upload::print (std::string tabulation) const {
	std::cout << root;
}

Index::Index (): SimpleComponent ("index") {}
Index::Index (const Index& _rhs): SimpleComponent (_rhs), indexes (_rhs.indexes) {}
Index& Index::operator= (const Index& _rhs) {
	if (this != &_rhs)
		indexes = _rhs.indexes;
	return *this;
}
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
AutoIndex::AutoIndex (const AutoIndex& _rhs): SimpleComponent (_rhs), on (_rhs.on) {}
AutoIndex& AutoIndex::operator = (const AutoIndex& _rhs) {
	if (this != &_rhs)
		on = _rhs.on;
	return *this;
}
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
BodySize::BodySize (const BodySize& _rhs): SimpleComponent (_rhs), size (_rhs.size), already_set (_rhs.already_set) {}
BodySize& BodySize::operator = (const BodySize& _rhs) {
	if (this != &_rhs) {
		size = _rhs.size;
		already_set = _rhs.already_set;
	}
	return *this;
}
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
AllowedMethods::AllowedMethods (const AllowedMethods& _rhs): SimpleComponent (_rhs), methods (_rhs.methods) {}
AllowedMethods& AllowedMethods::operator = (const AllowedMethods& _rhs) {
	if (this != &_rhs)
		methods = _rhs.methods;
	return *this;
}
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
ErrorPages::ErrorPages (const ErrorPages& _rhs):SimpleComponent (_rhs), error_pages (_rhs.error_pages) {}
ErrorPages& ErrorPages::operator = (const ErrorPages& _rhs) {
	if (this != &_rhs)
		error_pages = _rhs.error_pages;
	return *this;
}
ErrorPages::~ErrorPages () {}
ErrorPages* ErrorPages::clone () { return new ErrorPages (*this); }
bool ErrorPages::isSet () const { return !error_pages.empty (); }
void ErrorPages::parse (Tokenizer& tokenizer) {
	std::pair <std::map<unsigned short, std::string>::iterator, bool> ret;
	while (!(*tokenizer).is_directive ()) {
		unsigned short		code;
		std::string			scode;
		std::string			path;
		std::string token = (*tokenizer).id ();
		if (token.find (":") == std::string::npos)
			throw Error (tokenizer.error ("expecting XXX:/yyy/yy"));
		scode = token.substr (0, token.find (':'));
		path = token.substr (token.find (':') + 1);
		if (all_of_ (scode.begin (), scode.end (), is_digit ())) {
			code = atoi (scode.c_str ());
			if (code <= 0xFFFF) {
				if (!path.empty ()) {
					if (error_pages.insert (std::make_pair (code, path)).second == false)
						throw Error (tokenizer.error ("error page already set"));
				}
				else
					throw Error (tokenizer.error ("empty path"));
			}
			else
				throw Error (tokenizer.error ("code out of range"));
		}
		else
			throw Error (tokenizer.error ("expected a code"));
		++tokenizer;
	}
}
void ErrorPages::print (std::string tabulation) const {
	for (std::map <unsigned short, std::string>::const_iterator it = error_pages.cbegin (); it != error_pages.end (); ++it) {
		std::cout << it->first << ":" << it->second << " ";
	}
}

ServerNames::ServerNames (): SimpleComponent ("server_name") {}
ServerNames::ServerNames (const ServerNames& _rhs):SimpleComponent (_rhs), names (_rhs.names) {}
ServerNames& ServerNames::operator = (const ServerNames& _rhs) {
	if (this != &_rhs)
		names = _rhs.names;
	return *this;
}

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