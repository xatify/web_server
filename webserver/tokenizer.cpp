#include "tokenizer.hpp"
#include <fstream>

Tokenizer::Error::Error (const std::string& _error): error (_error) {}

const char* Tokenizer::Error::what () const _NOEXCEPT { return error.c_str (); }

Tokenizer::Tokenizer (const char *file) {
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
		current = tokens.begin ();
	}
	else
		std::logic_error ("couldn't open file");
}

Tokenizer::~Tokenizer () {}

Token Tokenizer::operator *() const { return *current; }

Tokenizer& Tokenizer::operator++ () { ++current; return *this; }

void Tokenizer::expect (const std::string& directive) {
	(*current).expect (directive);
	++current;
}

Tokenizer::operator bool ()  { return current == tokens.end (); }