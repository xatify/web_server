
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
		Component * HttpConfig = ComponentCreator::instance ().create ("http");
		try {
			HttpConfig->syntax_parse (tokenizer);
			HttpConfig->pretty_print ();
		}
		catch (std::exception& e) {
			std::cout << e.what () << std::endl;
		}
	}
}