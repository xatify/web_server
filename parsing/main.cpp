
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
			
			Root* root = dynamic_cast <Root*>(HttpConfig->getSimpleAttribute ("root"));
			root->setRoot ("coco");
			std::vector <Component*> servers = HttpConfig->getSubComponents ("server");

			for (std::vector<Component* >::iterator it = servers.begin (); it != servers.end (); ++it) {
				(*it)->pretty_print ();
			}
			root->pretty_print ();
			//HttpConfig->pretty_print ();
		}
		catch (std::exception& e) {
			std::cout << e.what () << std::endl;
		}
	}
}