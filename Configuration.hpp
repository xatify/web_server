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
		virtual ~Component ();
		const std::string& dir () const { return _dir; } 
		virtual bool isSet () const = 0;
		virtual Component* clone () = 0;
		virtual void print (std::string tabulation = "") const = 0;
		virtual void pretty_print (std::string tabulation = "") const = 0;
		virtual void syntax_parse (Tokenizer &) = 0;
		virtual void parse (Tokenizer& ) = 0;
};

class SimpleComponent: public Component {
	public:
		SimpleComponent (const std::string &, bool _sin = true);
		virtual ~SimpleComponent ();
		virtual bool isSet () const = 0;
		virtual SimpleComponent* clone () = 0;
		virtual void print (std::string _tab = "") const = 0;
		virtual void pretty_print (std::string _tab = "") const;
		virtual void syntax_parse (Tokenizer &);
		virtual void parse (Tokenizer& ) = 0;
};

class BracketedComponent: public Component {
	public:
		BracketedComponent (const std::string &, bool _sin = false);
		virtual ~BracketedComponent ();
		virtual BracketedComponent* clone () = 0;
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
		virtual ~SuffixBracketedComponent ();
		std::string&	_suffix ();
		virtual bool isSet () const = 0;
		virtual SuffixBracketedComponent* clone () = 0;
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
	private:
		Address _addr;
		Port	_prt;
	public:
		Listen ();
		~Listen ();
		Listen* clone ();
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
		Index* clone ();
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
		Root* clone ();
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
		AutoIndex* clone ();
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
		BodySize* clone ();
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
		AllowedMethods* clone ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void parse (Tokenizer& );
};

class ErrorPages: public SimpleComponent {
	private:
		std::map <unsigned short, std::string> error_pages;
	public:
		ErrorPages ();
		~ErrorPages ();
		ErrorPages* clone ();
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
		ServerNames* clone ();
		void print (std::string tabulation) const;
		bool isSet () const;
		bool empty () const;
		void parse (Tokenizer& );
};


class Location: public SuffixBracketedComponent {
	private:
		std::vector <Component *> _com;
	public:
		Location ();
		~Location ();
		Location& operator= (const Location&);
		Location* clone ();
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
		std::vector <Component *> _com;
		Locations	locations;
	public:
		Server ();
		~Server ();
		Server* clone ();
		Server& operator = (const Server&);
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