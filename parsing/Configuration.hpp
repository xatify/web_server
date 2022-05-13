#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include "tokenizer.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>

enum COMP {
	UNKNOWN			=		0,
	LISTEN 			=		1,
	ROOT			=		1 << 1,
	INDEX			=		1 << 2,
	AUTOINDEX		= 		1 << 3,
	BODYSIZE		=		1 << 4,
	ALLOWEDMETHODS	=		1 << 5,
	ERRORPAGES 		=		1 << 6,
	SERVERNAMES		=		1 << 7,
	LOCATION		=		1 << 8,
	SERVER			=		1 << 9,
	HTTPCONFIG		=		1 << 10,
	UPLOAD			=		1 << 11
};

class Component {
	protected:
		bool 			_sin;
		std::string		_dir;
		unsigned		_subs;
		std::multimap <std::string, Component *> _subComp;
	public:
		Component (const std::string &, bool sin = true, unsigned int = 0);
		Component (const Component &);
		bool sin () const { return _sin; }
		bool isSub (const std::string&) const;
		Component *getSimpleAttribute (const std::string& dir) const;
		std::vector<Component *> getSubComponents(const std::string& dir) const;
		Component& operator = (const Component&);
		virtual ~Component ();
		const std::string& dir () const { return _dir; } 
		virtual bool isSet () const;
		virtual void pretty_print (std::string tabulation = "") const = 0;
		virtual void syntax_parse (Tokenizer &) = 0;
};




class SimpleComponent: public Component {
	public:
		SimpleComponent (const std::string &, bool sin = true);
		SimpleComponent (const SimpleComponent &);
		SimpleComponent& operator = (const SimpleComponent&);
		virtual ~SimpleComponent ();
		virtual void print (std::string _tab = "") const = 0;
		virtual void parse (Tokenizer& ) = 0;
		virtual void pretty_print (std::string _tab = "") const;
		virtual void syntax_parse (Tokenizer &);
};

class BracketedComponent: public Component {
	public:
		BracketedComponent (const std::string &, bool sin, unsigned _sub);
		BracketedComponent (const BracketedComponent &);
		BracketedComponent& operator = (const BracketedComponent&);
		~BracketedComponent ();
		void pretty_print (std::string _tab = "") const;
		void syntax_parse (Tokenizer &);
};

class SuffixBracketedComponent: public Component {
	protected:
		std::string		_suf;
	public:
		SuffixBracketedComponent (const std::string &, bool sin, unsigned _sub);
		SuffixBracketedComponent (const SuffixBracketedComponent &);
		SuffixBracketedComponent& operator = (const SuffixBracketedComponent&);
		~SuffixBracketedComponent ();
		std::string&	_suffix ();
		void pretty_print (std::string _tab = "") const;
		void syntax_parse (Tokenizer &);
};

class Address {
	private:
		bool is_set;
		u_int32_t address;
	public:
		Address ();
		~Address ();
		Address (const Address& _rhs);
		Address& operator = (const Address&);
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
		Port (const Port&);
		Port& operator = (const Port&);
		~Port ();
		bool isSet () const { return is_set; }
		u_int16_t getPort () const;
		void setPort (u_int16_t prt);
		void parse (Tokenizer& );
};

class Listen: public SimpleComponent {
	private:
		Address _addr;
		Port	_prt;
	public:
		Listen ();
		Listen (const Listen&);
		Listen& operator = (const Listen&);
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
		Index (const Index&);
		Index& operator = (const Index& );
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
		Root (const Root&);
		Root& operator =(const Root&);
		~Root ();
		Root* clone ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void setRoot (const std::string& rt) { root = rt; };
		void parse (Tokenizer& );
};

class Upload: public SimpleComponent {
	private:
		std::string			root;
	public:
		Upload ();
		Upload (const Upload&);
		Upload& operator =(const Upload&);
		~Upload ();
		Upload* clone ();
		void print (std::string tabulation) const;
		bool isSet () const;
		void setRoot (const std::string& rt);
		void parse (Tokenizer& );
};
class AutoIndex: public SimpleComponent {
	private:
		std::string on;
	public:
		AutoIndex ();
		AutoIndex (const AutoIndex&);
		AutoIndex& operator = (const AutoIndex& );
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
		BodySize (const BodySize&);
		BodySize& operator = (const BodySize& );
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
		AllowedMethods (const AllowedMethods&);
		AllowedMethods& operator = (const AllowedMethods& );
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
		ErrorPages (const ErrorPages&);
		ErrorPages& operator = (const ErrorPages& );
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
		ServerNames (const ServerNames&);
		ServerNames& operator = (const ServerNames& );
		~ServerNames ();
		ServerNames* clone ();
		void print (std::string tabulation) const;
		bool isSet () const;
		bool empty () const;
		void parse (Tokenizer& );
};

class ComponentCreator {
	private:
		ComponentCreator () {};
		static std::map <std::string, COMP> mp; 
	public:
		COMP					id (const std::string & _dir) {
			std::map<std::string, COMP>::const_iterator it = mp.find (_dir);
			if (it == mp.end ()) return UNKNOWN;
			return (*it).second;
		}
		Component				*create (COMP c) {
			switch (c) {
				case LISTEN:
					return new Listen ();
				case ROOT:
					return new Root ();
				case INDEX:
					return new Index ();
				case AUTOINDEX:
					return new AutoIndex ();
				case BODYSIZE:
					return new BodySize ();
				case ALLOWEDMETHODS:
					return new AllowedMethods ();
				case ERRORPAGES:
					return new ErrorPages ();
				case SERVERNAMES:
					return new ServerNames ();
				case LOCATION:
					return new SuffixBracketedComponent ("location", false, ROOT | INDEX | AUTOINDEX | ALLOWEDMETHODS | UPLOAD);
				case SERVER:
					return new BracketedComponent ("server", false, LISTEN | ROOT | INDEX | ERRORPAGES | SERVERNAMES | BODYSIZE | LOCATION);
				case HTTPCONFIG:
					return new BracketedComponent ("http", true, INDEX | ROOT | SERVER);
				case UPLOAD:
					return new Upload ();
				default:
					return 0x0;
			}
		}
		Component				*create (const std::string& _dir) { return create (id (_dir)); } 
		
		static ComponentCreator& instance () {
			static ComponentCreator *singleton = new ComponentCreator ();
			return *singleton;
		}
};

#endif