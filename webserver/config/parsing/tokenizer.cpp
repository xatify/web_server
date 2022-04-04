#include "tokenizer.hpp"


Tokenizer::Error::Error (const std::string& _error): error (_error) {}

const char* Tokenizer::Error::what () const _NOEXCEPT { return error.c_str (); }


Tokenizer::Tokenizer (const char *file): line (0), column (0) {
	if (fb.open (file, std::ios::in)) {
		std::istream is (&fb);
		std::noskipws (is);
		it = std::istream_iterator <char> (is);
	}
	else
		throw Error (std::string ("no file named ") + file);
}

Tokenizer::~Tokenizer () {
	fb.close ();
}

void Tokenizer::skip_comment () {
	if (*it == '#') {
		while ((it != end && *it != '\n')) {
				column++;
				++it;
		}
	}
}

Token Tokenizer::next_token() {
	token.clear ();
	while (it != end) {
		if (*it == '#') skip_comment ();
		else if (*it == '\n') {
			line++;
			column = 0;
			++it;
		}
		else if (isspace (*it)) {
			++it;
			column++;
			if (!token.empty ())
				return (Token(token, line, (column - 1) - token.size ()));
 		}
		else {
			if (*it == ';') {
				if (!token.empty ()) {
					Token tkn (token, line, column - token.size ());
					token.empty ();
					return (Token(token, line, column - token.size ()));
				}
				else {
					++it;
					column++;
					return Token (";", line, column - 1);
				}
			}
			else {
				token += *it;
				column++;
				++it;
			}
		}
 	}
	return Token ("", line, column);
}