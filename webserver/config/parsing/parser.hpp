#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "../configuration/configuration.hpp"

#include <exception>
#include "tokenizer.hpp"


template <typename T>
class Parser {
	private:
		T			*component;
	public:
		Parser (): component (0x0) {}
		~Parser () { delete component; };
		T&	parse (Tokenizer& tokenizer);
};

template <>
HttpConfig&	Parser<HttpConfig>::parse (Tokenizer& tokenizer) {
	component = new HttpConfig;
	
	tokenizer.expect ("http");
	tokenizer.expect ("{");

	Parser<Index>	indexParser;
	Parser<Root>	rootParser;
	Parser<Server>	serverParser;
	while (tokenizer) {
		std::string directive (tokenizer.current_token ().id ());
		if (directive ==  "index") {
			if (component->getIndex ().empty ())
				component->getIndex () = indexParser.parse (tokenizer);
			else
				throw std::logic_error ("no more than one index allowed");
		}
		else if (directive == "root") {
			if (component->getRoot ().empty()) {
				component->getRoot () = rootParser.parse (tokenizer);
			}
			else
				throw std::logic_error ("no more than one root allowed");
		}
		else if (directive == "server") {
			component->getServers ().push_back (serverParser.parse (tokenizer));
		}
		else
			throw std::logic_error ("directive Unkown");
	}
	tokenizer.expect ("}");
	return *component;
}

template <>
Index& Parser<Index>::parse (Tokenizer& tokenizer) {
	component = new Index;
	
	while (tokenizer) {
		std::
	}
}

#endif