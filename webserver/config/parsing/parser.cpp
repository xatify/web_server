#include "parser.hpp"


Parser::Parser (void) {}
Parser::~Parser () {}

ConfigParser::ConfigParser (): config (0x0) {}
ConfigParser::~ConfigParser () { delete config; }

void ConfigParser::parse (Tokenizer& tokenizer) {

	config = new HttpConfig;
	tokenizer.expect ("http");
	tokenizer.expect ("{");
	while (tokenizer) {
		if (tokenizer.current_token ().id () ==  "index") {
			if (config->getIndex ().empty ()) {
				IndexParser indexParser;
				indexParser.parse (tokenizer);
				config->getIndex () = *indexParser.clone ();
			}
			else {

			}
		}
	}	
	tokenizer.expect ("}");

}


IndexParser::IndexParser