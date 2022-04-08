
#include "tokens.hpp"
#include <sstream>
#include "utils.hpp"

Token::Token (std::string _token, unsigned int _line, unsigned int _column): token (_token), ln (_line), clmn (_column) {}

unsigned int& Token::column (void) { return clmn; }

unsigned int& Token::line (void)  { return ln; }

bool Token::empty () const { return token.empty (); }

std::string Token::status (const char *stts) const {
	std::stringstream str;
	if (stts)
		str << stts;
	str << " at (line: " << ln << ",column: " << clmn << ")";
	return str.str ();
}

void Token::expect (const std::string& id) {
	if (token != id) {
		std::string s ("expecting token '");
		s += id ;
		s += "' at ";
		throw std::logic_error (status (s.c_str ()));
	}
}

const std::string& Token::id () const { return token; }

const char* Token::directives [] = {";", "{", "}", "root", "index", "listen", "error_page", "auto_index", "server_name", 
									"allowed_methods", "max_body_size", "location", "server"};

bool Token::is_directive () const {
	for (unsigned long i = 0; i < sizeof (directives) / sizeof (char *); ++i) {
		if (token == directives[i]) return true;
	}
	return false;
}

bool Token::isNumber () const {
	return all_of_ (token.begin (), token.end (), is_digit ());
}