#include "tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>

Error::Error (const std::string& _error): error (_error) {}

const char* Error::what () const _NOEXCEPT { return error.c_str (); }


void Tokenizer::add_token (std::string& tkn, unsigned int line, unsigned int column) {
	if (!tkn.empty ()) {
		Token token (tkn, line, column - tkn.size ());
		if (!tokens.empty ()) {
			std::string last = tokens.back ().id ();
			if ((last == ";" && tkn == ";") ||
				(last == "{" && tkn == "{") ||
				(last == "{" && tkn == ";") ||
				(last == "}" && tkn == ";")) {
				throw Error (token.status("unexpected token"));
			}
		}
		tokens.push_back (token);
		tkn.clear ();
	}
}

Tokenizer::Tokenizer (const char *_file): file (_file), _ready (false) {}

Tokenizer::~Tokenizer () {}

void Tokenizer::tokenize () {
	std::ifstream is (file, std::ifstream::in);
	if (is.is_open ()) {
		unsigned int line =1;
		unsigned int column =1;
		typedef std::istream_iterator <char> streamer;
		std::noskipws (is);
		std::string token;
		for (streamer it = is; it != streamer (); ++it, ++column) {
			switch (*it) {
				case '#':
					while ((it != streamer () && *it != '\n'))
						++it;
				case '\n':
					add_token (token, line, column);
					line++;
					column = 0;
					break;
				case ';':
				case '}':
				case '{':
					add_token (token, line, column);
					token += *it;
					add_token (token, line, column + 1);
					break;
				default:
					if (isspace (*it)) {
						add_token (token, line, column);
						if (*it == '\t') column += 3;
						break;
					}
					token += *it;
			}
		}
		add_token (token, line, column);
		_ready = true;
		is.close ();
		current = tokens.cbegin ();
	}
	else
		throw Error ("couldn't open file");
}

const Token& Tokenizer::operator *() const { return *current; }

Tokenizer& Tokenizer::operator++ () { ++current; return *this; }

void Tokenizer::expect (const std::string& directive) {
	(*current).expect (directive);
	++current;
}

bool Tokenizer::ready () const { return _ready;  }

Tokenizer::operator bool ()  { return current != tokens.cend (); }

void Tokenizer::print_tokens () const {
	for (std::vector<Token>::const_iterator it = tokens.cbegin (); it != tokens.cend (); ++it) {	
		std::cout << *it;
	}
}