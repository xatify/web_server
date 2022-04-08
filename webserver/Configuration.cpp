#include "Configuration.hpp"
#include <arpa/inet.h>
#include "utils.hpp"

// must respect network byte order
Address::Address (): is_set (false), address (0) {}
Address::~Address () {}
u_int32_t Address::getAdress () const { return address; }
void Address::setAddress (u_int32_t addr) { address = addr; }
void Address::parse (Tokenizer& tokenizer) {
	std::string address = (*tokenizer).id ();
	if (address.find ('.')){
		if (inet_pton (AF_INET, address.c_str (), &address) == 1) {
			++tokenizer;
			is_set = true;
			return;
		}
		throw Tokenizer::Error ((*tokenizer).status ("expected address"));
	}
}

Port::Port (): port (0) {}
Port::~Port () {}
u_int16_t Port::getPort () const { return port; }
void Port::setPort (u_int16_t prt) { port = prt; }
void Port::parse (Tokenizer& tokenizer) {
	std::string address = (*tokenizer).id ();
	if (all_of_ (address.begin (), address.end (), is_digit ())) {
		unsigned int prt = std::atoi (address.c_str ());
		if (prt <= 0xFFFF) {
			port = prt;
			++tokenizer;
		}
		else
			throw Tokenizer::Error ((*tokenizer).status ("wrong port"));
	}
}

Listen::Listen (): Component ("listen") {}
Listen::~Listen () {}
bool Listen::isSet () const { return Address::isSet () || Port::isSet (); }
void Listen::parse (Tokenizer& tokenizer) {
	Address::parse (tokenizer);
	Port::parse (tokenizer);
}


Root::Root (): Component ("root") {}
Root::~Root () {}
bool Root::isSet () const { return root.empty (); }
void Root::parse (Tokenizer &tokenizer) {
	if ((*tokenizer).is_directive ())
		Tokenizer::Error ((*tokenizer).status ("expected token"));
	root = (*tokenizer).id ();
}

Index::Index (): Component ("index") {}
Index::~Index () {}
bool Index::isSet () const { return indexes.empty (); }
void Index::parse (Tokenizer & tokenizer) {
	while (!(*(++tokenizer)).is_directive ())
		indexes.push_back ((*tokenizer).id ());
}

AutoIndex::AutoIndex (): Component ("autoindex"), on ("false") {}
AutoIndex::~AutoIndex () {}
bool AutoIndex::isSet () const { return on.empty (); }
void AutoIndex::parse (Tokenizer& tokenizer) {
	if ((*tokenizer).id () == "on" || (*tokenizer).id () == "off")
		on = (*tokenizer).id ();
	else
		throw Tokenizer::Error ((*tokenizer).status ("expected on/off"));
}

BodySize::BodySize (): Component ("max_body_size"), size (0), already_set (false) {}
BodySize::~BodySize () {}
void BodySize::setBodySize (unsigned int sz) { size = sz; } 
unsigned int BodySize::getBodySize () const { return size; }
bool BodySize::isSet () const { return already_set; }
void BodySize::parse (Tokenizer& tokenizer) {
	std::string sz = (*tokenizer).id ();
	if (sz.back() == 'M' && all_of_ (sz.begin (), sz.end () - 1, is_digit ())) {
		already_set = true;
		size = atoi (sz.c_str ()) * 1024;
	}
}

AllowedMethods::AllowedMethods (): Component ("allowed_methods") {}
AllowedMethods::~AllowedMethods () {}
bool AllowedMethods::isSet () const { return methods.empty (); }
void AllowedMethods::parse (Tokenizer& tokenizer) {
	std::pair<std::set<std::string>::iterator,bool> ret;
	while (!(*(++tokenizer)).is_directive ()) {
		ret = methods.insert ((*tokenizer).id ());
		if (ret.second == true)
			throw Tokenizer::Error ((*tokenizer).status ("multiple methods with same name"));
	}
}

ErrorPages::ErrorPages (): Component ("error_page", false) {}
ErrorPages::~ErrorPages () {}
bool ErrorPages::isSet () const { return error_pages.empty (); }
void ErrorPages::parse (Tokenizer& tokenizer) {
	unsigned int code;
	std::string		path;
	if ((*tokenizer).isNumber ()) {
		code = atoi ((*tokenizer).id ().c_str ());
		if (code <= 0xFFFF) {
			++tokenizer;
			if (!(*tokenizer).is_directive ()) {
				path = (*tokenizer).id ();
				if (error_pages.insert (std::make_pair (code, path)).second)
					throw Tokenizer::Error ((*tokenizer).status ("error page already set"));
				return;
			}
			throw Tokenizer::Error ((*tokenizer).status ("expected path"));
		}
		throw Tokenizer::Error ((*tokenizer).status ("code out of range"));
	}
	throw Tokenizer::Error ((*tokenizer).status ("expected a number"));
}

ServerNames::ServerNames (): Component ("server_name") {}
ServerNames::~ServerNames () {}
bool ServerNames::isSet () const { return names.empty (); }
void ServerNames::parse (Tokenizer& tokenizer) {
	std::pair<std::set<std::string>::iterator,bool> ret;
	while (!(*(++tokenizer)).is_directive ()) {
		ret = names.insert ((*tokenizer).id ());
		if (ret.second == true)
			throw Tokenizer::Error ((*tokenizer).status ("same name Server_Names"));
	}
}

Location::Location (): Component ("location", false, true, true) {}
Location::~Location () {}
std::string Location::path () const { return afdirective (); }
bool Location::isSet () const { return root.isSet () || index.isSet () || autoIndex.isSet () || methods.isSet ();}
void Location::parse (Tokenizer& tokenizer) {
	while (tokenizer && (*tokenizer).id () != "}") {
		index.parse (tokenizer);
		root.parse (tokenizer);
		autoIndex.parse (tokenizer);
		methods.parse (tokenizer);
	}
}

Locations::Locations () {}
Locations::~Locations () {}
bool Locations::isSet () const { return locations.empty (); }
void Locations::parse (Tokenizer& tokenizer) {
	Location location;
	location.parse (tokenizer);
	if (location.isSet ())
		locations.push_back (location);
}

Server::Server (): Component ("server", false, false, true) {}
Server::~Server () {}
bool Server::isSet () const { return root.isSet () || index.isSet () || listen.isSet ()
							|| error_pages.isSet () || server_names.isSet () || body_size.isSet ()
							|| locations.isSet (); }
void Server::parse (Tokenizer& tokenizer) {
	while (tokenizer && (*tokenizer).id () != "}") {
		listen.parse (tokenizer);
		root.parse (tokenizer);
		index.parse (tokenizer);
		error_pages.parse (tokenizer);
		server_names.parse (tokenizer);
		body_size.parse (tokenizer);
		locations.parse (tokenizer);
	}
}

vServers::vServers () {}
vServers::~vServers () {}
bool vServers::isSet () const { return servers.empty (); }
void vServers::parse (Tokenizer& tokenizer){
	Server server;
	server.parse (tokenizer);
	if (server.isSet ())
		servers.push_back (server);
}

HttpConfig::HttpConfig (): Component ("http", true, false, true) {}
HttpConfig::~HttpConfig () {}
bool HttpConfig::isSet () const { return index.isSet () || root.isSet () || servers.isSet (); }
void HttpConfig::parse (Tokenizer& tokenizer) {
	while (tokenizer && (*tokenizer).id () != "}") {
		index.parse (tokenizer);
		root.parse (tokenizer);
		servers.parse (tokenizer);
	}
}