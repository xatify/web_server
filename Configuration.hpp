#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include "tokenizer.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>

class Component {
	protected:
		std::string		_dir;
		bool			_sin;
	public:
		Component (const std::string &, bool);
		~Component ();
		virtual bool isSet () const = 0;
		virtual void print (std::string tabulation = "") const = 0;
		virtual void pretty_print (std::string tabulation = "") const = 0;
		virtual void syntax_parse (Tokenizer &) = 0;
		virtual void parse (Tokenizer& ) = 0;
};

class SimpleComponent: public Component {
	public:
		SimpleComponent (const std::string &, bool _sin = true);
		~SimpleComponent ();
		virtual bool isSet () const = 0;
		virtual void print (std::string _tab = "") const = 0;
		virtual void pretty_print (std::string _tab = "") const;
		virtual void syntax_parse (Tokenizer &);
		virtual void parse (Tokenizer& ) = 0;
};

class BracketedComponent: public Component {
	public:
		BracketedComponent (const std::string &, bool _sin = false);
		~BracketedComponent ();
		virtual bool isSet () const = 0;
		virtual void print (std::string _tab = "") const = 0;
		virtual void pretty_print (std::string _tab = "") const;
		virtual void syntax_parse (Tokenizer &);
		virtual void parse (Tokenizer& ) = 0;

};

class SuffixBracketedComponent: public Component {
	protected:
		std::string		_suf;
	public:
		SuffixBracketedComponent (const std::string &, bool _sin = false);
		~SuffixBracketedComponent ();
		std::string&	_suffix ();
		virtual bool isSet () const = 0;
		virtual void print (std::string _tab = "") const = 0;
		virtual void pretty_print (std::string _tab = "") const;
		virtual void syntax_parse (Tokenizer &);
		virtual void parse (Tokenizer& ) = 0;
};

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

class Listen: public SimpleComponent, Port, Address {
	public:
		Listen ();
		~Listen ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void parse (Tokenizer& );
};

class Index: public SimpleComponent {
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

class Root: public SimpleComponent {
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

class AutoIndex: public SimpleComponent {
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

class BodySize: public SimpleComponent {
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

class AllowedMethods: public SimpleComponent {
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

class ErrorPages: public SimpleComponent {
	private:
		std::map <unsigned short, std::string> error_pages;
	public:
		ErrorPages ();
		~ErrorPages ();
		bool isSet () const;
		void print (std::string tabulation) const;
		void parse (Tokenizer& );
};

class ServerNames: public SimpleComponent {
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


class Location: public SuffixBracketedComponent {
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

class Server: public BracketedComponent {
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

class HttpConfig: public BracketedComponent {
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