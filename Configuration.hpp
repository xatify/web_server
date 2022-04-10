#ifndef __CONFIGURATION_HPP_
#define __CONFIGURATION_HPP_

#include "ConfiGrammar.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <set>

class Address {
	private:
		bool is_set;
		u_int32_t address;
	public:
		Address ();
		~Address ();
		bool isSet () const { return is_set; }
		const char* print () const;
		u_int32_t getAdress () const;
		void setAddress (u_int32_t addr);
		void parse (Tokenizer& );
};

class Port {
	private:
		bool is_set;
		u_int16_t port;
	public:
		Port ();
		~Port ();
		bool isSet () const { return is_set; }
		u_int16_t getPort () const;
		void setPort (u_int16_t prt);
		void parse (Tokenizer& );
};

class Listen: public Component, Port, Address {
	public:
		Listen ();
		~Listen ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void parse (Tokenizer& );
};

class Index: public Component {
	private:
		std::vector <std::string>	indexes;
	public:
		Index ();
		~Index ();
		void print (std::string tabulation) const;
		Index& operator = (const Index& idx);
		bool isSet () const;
		void addIndex (const std::string& idx);
		void parse (Tokenizer& );
};

class Root: public Component {
	private:
		std::string			root;
	public:
		Root ();
		~Root ();
		void print (std::string tabulation) const;
		const Root& operator = (const Root& rt);
		bool isSet () const;
		void setRoot (const std::string& rt);
		void parse (Tokenizer& );
};

class AutoIndex: public Component {
	private:
		std::string on;
	public:
		AutoIndex ();
		~AutoIndex ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void setAutoIndex (bool on);
		bool getAutoIndex () const;
		void parse (Tokenizer& );
};

class BodySize: public Component{
	private:
		unsigned int size;
		bool already_set; 
	public:
		BodySize ();
		~BodySize ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void setBodySize (unsigned int);
		unsigned int getBodySize () const;
		void parse (Tokenizer& );
};

class AllowedMethods: public Component {
	private:
		std::set <std::string> methods;
	public:
		AllowedMethods ();
		~AllowedMethods ();
		void print (std::string tabulation) const;
		friend std::ostream& operator<< (std::ostream& os, const Address&);
		bool isSet () const;
		void parse (Tokenizer& );
};

class ErrorPages: public Component {
	private:
		std::map <unsigned short, std::string> error_pages;
	public:
		ErrorPages ();
		~ErrorPages ();
		bool isSet () const;
		void print (std::string tabulation) const;
		void parse (Tokenizer& );
};

class ServerNames: public Component {
	private:
		std::set <std::string>	names;
	public:
		ServerNames ();
		~ServerNames ();
		void print (std::string tabulation) const;
		bool isSet () const;
		bool empty () const;
		void parse (Tokenizer& );
};


class Location: public Component {
	private:
		Root			root;
		Index			index;
		AutoIndex		autoIndex;
		AllowedMethods	methods;
	public:
		Location ();
		~Location ();
		void print (std::string tabulation) const;
		std::string path () const;
		bool isSet () const;
		bool empty () const;
		void parse (Tokenizer& );
};

class Locations {
	private:	
		std::vector<Location> locations;
	public:
		Locations ();
		~Locations ();
		bool isSet () const;
		void print (std::string tabulation) const;
		void GetLocations () const;
		void parse (Tokenizer& );
};

class Server: public Component {
	private:
		Root		root;
		Index		index;
		Listen		listen;
		ErrorPages	error_pages;
		ServerNames	server_names;
		BodySize	body_size;
		Locations	locations;
	public:
		Server ();
		~Server ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void parse (Tokenizer&);
};

class vServers {
	private:
		std::vector<Server> servers;
	public:
		vServers ();
		~vServers ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void parse (Tokenizer& );
};

class HttpConfig: public Component {
	private:
		Index		index;
		Root		root;
		vServers	servers;
	public:
		HttpConfig ();
		~HttpConfig ();
		void print (std::string tabulation = "") const;
		bool isSet () const;
		void parse (Tokenizer &);
};



#endif