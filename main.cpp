
#include "Configuration.hpp"
#include <iostream>
#include <fstream>

int main () {
	Tokenizer tokenizer ("./abc.txt");
	
	try {
		tokenizer.tokenize ();
	}
	catch (std::exception& e) {
		std::cout << e.what ();
	}
	if (tokenizer.ready ()) {
		HttpConfig Config;
		try {
			Config.syntax_parse (tokenizer);
			Config.pretty_print ();
		}
		catch (std::exception& e) {
			std::cout << e.what ();
		}
	}
}