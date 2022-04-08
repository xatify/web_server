
#include "tokenizer.hpp"
#include <iostream>
#include <fstream>


void okenizer (const char *file) {
	std::vector<Token> tokens;
	std::ifstream	is;
	is.open (file);
	if (is.is_open ()) {
		unsigned int line =0;
		unsigned int column =0;
		std::noskipws (is);
		std::istream_iterator <char> eof;
		std::istream_iterator <char> it (is);
		std::string token;
		while (it != eof ) {
			if (*it == '#') {
				while ((it != eof && *it != '\n')) {
					column++;
					++it;
				}
			}
			if (*it == '\n') {
				line++;
				column = 0;
			}
			if (isspace (*it)) {
				if (!token.empty ()) {
					tokens.push_back (Token (token, line, column - token.size ()));
					token.clear ();
				}
			}
			else {
				if (*it == ';') {
					if (!token.empty ()) {
						tokens.push_back (Token (token, line, column - token.size ()));
						token.clear ();
					}
					else
						tokens.push_back (Token (";", line, column));
				}
				else
					token += *it;
			}
			++it;
			column++;
		}
		is.close ();
	}
	std::cout << tokens.size ();
}


int main () {
	std::cout << "hello";
	okenizer ("abc.txt");
	// Tokenizer *tokenizer_ptr;
	// try {
	// 	tokenizer_ptr = new Tokenizer ("abc.txt");
	// }
	// catch (std::exception& e) {
	// 	std::cout << e.what ();
	// 	exit (1);
	// }
	// Tokenizer& tokenizer=  *tokenizer_ptr;
	// while (tokenizer) {
	// 	std::cout << (*tokenizer).id () << " ";
	// 	++tokenizer;
	// }

}