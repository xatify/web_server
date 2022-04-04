
#include "configuration.hpp"

// Http Config
void HttpConfig::add_index (std::string idx) { index.push_back (idx); }

const std::vector<std::string>& HttpConfig::get_index (void) const { return index; }

void HttpConfig::set_root (std::string rt) { root = rt; }

const std::string& HttpConfig::get_root (void) const { return root; }

void HttpConfig::add_server (Server srvr) { vServers.push_back (srvr); }

const std::vector<Server>& HttpConfig::get_servers () const { return vServers; }

// server Config

void Server::add_index (std::string idx) { index.push_back (idx); }

const std::vector<std::string>& Server::get_index (void) const { return index; }

void Server::set_root (std::string rt) { root = rt; }

const std::string& Server::get_root (void) const { return root; }


void Server::set_listen_host (u_int32_t ip) { listen.first = ip; }

u_int32_t Server::get_listen_host (void) const { return listen.first; }

void Server::set_listen_port (u_int16_t port) { listen.second = port; }

u_int16_t Server::get_listen_port (void) const { return listen.second; }

const std::vector<std::string>& Server::get_server_names (void) const { return serverNames; }

void	Server::add_server_name (std::string srvr) { serverNames.push_back (srvr); }

void Server::add_error_page (unsigned short code, const std::string& path) { error_page[code] = path; }

unsigned int Server::get_client_max_body_size (void) const { return client_max_body_size; }

void Server::set_client_max_body_size (unsigned int size) { client_max_body_size = size; }

// location 
void Location::add_index (std::string idx) { index.push_back (idx); }

const std::vector<std::string>& Location::get_index (void) const { return index; }

void Location::set_root (std::string rt) { root = rt; }

const std::string& Location::get_root (void) const { return root; }

bool Location::get_auto_index (void) const { return autoindex; }

void Location::set_auto_index (bool on) { autoindex = on; }

const std::vector<std::string>& Location::get_allowed_methodes (void) const{ return allowed_methods; }

void Location::add_allowed_method (const std::string& method) { allowed_methods.push_back (method); }


// Listen 

Listen::Listen (): address (0), port (80) {}

void Listen::set_address (u_int32_t add) { address = add; }

void Listen::set_port (u_int16_t prt) { port = prt; }

u_int32_t Listen::get_address (void) const { return address; }

u_int16_t Listen::get_port (void) const { return port; }



