
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>

std::vector<std::string> * tokenizer (const char *file) {
	std::filebuf fb;
	
	if (fb.open (file, std::ios::in)) {
		std::istream is (&fb);
		
		std::noskipws (is);
		std::istream_iterator <char> eos;
		std::istream_iterator <char> iit (is);

		std::vector<std::string> * tokens = new std::vector <std::string> ();

		bool start = false;
		std::string token;

		// iterat over the character
		// if it's a comment (beggining with #) skiip all that line
		// if it's a white space :check if token is not empty 
		// if not append it to vector of tokens
		// else if it's a normal character 
		// append it to the current token
		while (iit != eos) {
			if (*iit == '#')
				while ((iit != eos) && *iit != '\n') ++iit;
			else if (*iit == '\n')
				++iit;
			else if (isspace (*iit)) { 
				if (!token.empty ()) {
					tokens->push_back (token);
					token.clear ();
				}
				++iit;
			}
			else {
				if (*iit == ';') {
					if (!token.empty ()) {
						tokens->push_back (token);
						token.clear ();
					}
					tokens->push_back (";");
				}
				else
					token += *iit;
				++iit;
			}
		}
		fb.close ();
		return tokens;
	}
	return 0x0;
}

void print_tokens (std::vector<std::string> *tokens) {
	for (std::vector<std::string>::iterator it = tokens->begin (); it != tokens->end (); ++it)
				std::cout << *it << " ";
}

int main (int argc, char **argv) {
	if (argc < 2) return 1;

	std::vector<std::string> * tokens = tokenizer (argv[1]);

	return 0;
}