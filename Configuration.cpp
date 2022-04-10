#include "Configuration.hpp"
#include <arpa/inet.h>
#include "utils.hpp"

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

Listen::Listen (): Component ("listen") {}
Listen::~Listen () {}
bool Listen::isSet () const { return Address::isSet () || Port::isSet (); }
void Listen::parse (Tokenizer& tokenizer) {
	Address::parse (tokenizer);
	Port::parse (tokenizer);
}
void Listen::print (std::string tabulation) const {
	std::cout << Address::print() << ":" << getPort ();
}


Root::Root (): Component ("root") {}
Root::~Root () {}
bool Root::isSet () const { return !root.empty (); }
void Root::parse (Tokenizer &tokenizer) {
	if (!(*tokenizer).is_directive ()) {
		root = (*tokenizer).id ();
		++tokenizer;
	}
}
void Root::print (std::string tabulation) const {
	std::cout << root;
}


Index::Index (): Component ("index") {}
Index::~Index () {}
bool Index::isSet () const { return !indexes.empty (); }
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

AutoIndex::AutoIndex (): Component ("auto_index"), on ("") {}
AutoIndex::~AutoIndex () {}
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

BodySize::BodySize (): Component ("max_body_size"), size (0), already_set (false) {}
BodySize::~BodySize () {}
void BodySize::setBodySize (unsigned int sz) { size = sz; } 
unsigned int BodySize::getBodySize () const { return size; }
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

AllowedMethods::AllowedMethods (): Component ("allowed_methods") {}
AllowedMethods::~AllowedMethods () {}
bool AllowedMethods::isSet () const { return !methods.empty (); }
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

ErrorPages::ErrorPages (): Component ("error_page", false) {}
ErrorPages::~ErrorPages () {}
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

ServerNames::ServerNames (): Component ("server_name") {}
ServerNames::~ServerNames () {}
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

Location::Location (): Component ("location", false, true, true) {}
Location::~Location () {}
std::string Location::path () const { return afdirective (); }
bool Location::isSet () const {
	return root.isSet () || index.isSet () || autoIndex.isSet () || methods.isSet (); }
void Location::parse (Tokenizer& tokenizer) {
	while (tokenizer && tokenizer.id () != "}") {
		const std::string &direct = (*tokenizer).id ();
		if (direct == "index")
			index.syntax_parse (tokenizer);
		else if (direct == "root")
			root.syntax_parse (tokenizer);
		else if (direct == "auto_index")
			autoIndex.syntax_parse (tokenizer);
		else if (direct == "allowed_methods")
			methods.syntax_parse (tokenizer);
		else
			throw Error (tokenizer.error ("directive not allowed"));
	}
}

void Location::print (std::string tabulation) const {
	root.pretty_print (tabulation);
	index.pretty_print (tabulation);
	autoIndex.pretty_print (tabulation);
	methods.pretty_print (tabulation);
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

Server::Server (): Component ("server", false, false, true) {}
Server::~Server () {}
bool Server::isSet () const { return root.isSet () || index.isSet () || listen.isSet ()
							|| error_pages.isSet () || server_names.isSet () || body_size.isSet ()
							|| locations.isSet (); }
void Server::parse (Tokenizer& tokenizer) {
	while (tokenizer && tokenizer.id () != "}") {
		const std::string &direct = (*tokenizer).id ();
		if (direct == "listen")
			listen.syntax_parse (tokenizer);
		else if (direct == "root")
			root.syntax_parse (tokenizer);
		else if (direct == "index")
			index.syntax_parse (tokenizer);
		else if (direct == "error_page")
			error_pages.syntax_parse (tokenizer);
		else if (direct == "server_name")
			server_names.syntax_parse (tokenizer);
		else if (direct == "max_body_size")
			body_size.syntax_parse (tokenizer);
		else if (direct == "location")
			locations.parse (tokenizer);
		else
			throw Error (tokenizer.error ("directive not allowed"));
	}
}
void Server::print (std::string tabulation) const {
	listen.pretty_print (tabulation);
	root.pretty_print (tabulation);
	index.pretty_print (tabulation);
	error_pages.pretty_print (tabulation);
	server_names.pretty_print (tabulation);
	body_size.pretty_print (tabulation);
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


HttpConfig::HttpConfig (): Component ("http", true, false, true) {}
HttpConfig::~HttpConfig () {}
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