#ifndef __CONFIGRAMMAR_HPP__
#define __CONFIGRAMMAR_HPP__

#include <string>
#include "tokenizer.hpp"
// directive : the name of the component
// bracketed :	if true directive (after_direcive) { .. }
//				else directive ...;
class Component {
	private:
		std::string		directive;
		std::string  	af_directive;
		bool only_one;
		bool after_directive;
		bool bracketed;
	public:
		Component (const std::string& directive, bool one = true, bool after_dir = false, bool brackets = false);
		virtual ~Component () {};
		std::string afdirective () const { return af_directive; }
		virtual bool isSet () const = 0;
		virtual void syntax_parse (Tokenizer &);
		virtual void parse (Tokenizer& ) = 0;
};


#endif