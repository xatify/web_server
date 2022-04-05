
#include "tokens.hpp"
#include <sstream>

Token::Token (std::string _token = "", u_int32_t _line = 0, u_int32_t _column): token (_token), ln (_line), clmn (_column) {}

unsigned int& Token::column (void) { return clmn; }

unsigned int& Token::line (void)  { return ln; }

bool Token::empty () const { return token.empty (); }

void Token::clear () { token.clear (); }

void Token::append (const CharStreamer& stream) {
	if (!stream.empty ()) {
		if (token.empty ()) {
			ln = stream.line ();
			clmn = stream.column ();
		}
		token += *stream;
	}
}

std::string Token::status (const char *stts) const {
	std::stringstream str;
	if (stts)
		str << stts;
	str << "(line: " << ln << ",column: " << clmn << ")";
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
