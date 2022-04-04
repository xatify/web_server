
#ifndef __CONFIGURATION__
#define __CONFIGURATION__

#include <string>
#include <vector>
#include <map>

// CGI configuration
class CGI {};

class Location {
	private:
		std::string					root;
		std::vector <std::string>	index;
		bool						autoindex;
		std::vector<std::string>	allowed_methods;
		CGI 						cgi;
	public:
		// index
		void add_index (std::string idx);
		const std::vector <std::string>& get_index (void) const;
		
		// root
		void set_root (std::string rt);
		const std::string& get_root () const;

		// autoindex
		bool get_auto_index (void) const;
		void set_auto_index (bool);

		// allowed method
		const std::vector<std::string>& get_allowed_methodes (void) const;
		void add_allowed_method (const std::string& method);

		// CGI not understood yet;
};


class Listen {
	private:
		u_int32_t	address;
		u_int16_t	port;
	public:
		Listen ();
		void set_address (u_int32_t add);
		void set_port (u_int16_t);
		u_int32_t	get_address () const;
		u_int16_t	get_port () const;
};

class Server {
	private:
		std::pair <u_int32_t, u_int16_t> 		listen;
		std::vector <std::string> 				serverNames;
		std::string								root;
		std::vector <std::string>				index;
		std::map <unsigned short, std::string>  error_page;
		unsigned int 							client_max_body_size;
	public:
		// index
		void add_index (std::string idx);
		const std::vector <std::string>& get_index (void) const;
		
		// root
		void set_root (std::string rt);
		const std::string& get_root () const;
		
		// listen host ip
		void set_listen_host (u_int32_t ip);
		uint32_t get_listen_host (void) const;
		
		// listen on port
		void set_listen_port (u_int16_t port);
		u_int16_t get_listen_port (void) const;
		
		// server names
		const std::vector<std::string>& get_server_names (void) const;
		void					 add_server_name (std::string srv);
		
		// error_pages
		void add_error_page (unsigned short, const std::string&);
		const std::map <unsigned short, std::string>& get_error_page (void) const;

		// client max body size
		unsigned int	get_client_max_body_size (void) const;
		void set_client_max_body_size (unsigned int size);
};

class HttpConfig  {
	private:
		std::vector <std::string>	index;
		std::string					root;
		std::vector<Server>			vServers;
	public:
		void add_index (std::string idx);
		const std::vector <std::string>& get_index (void) const;
		void set_root (std::string rt);
		const std::string& get_root () const;
		void add_server (Server srvr);
		const std::vector<Server>& get_servers () const;
};

#endif