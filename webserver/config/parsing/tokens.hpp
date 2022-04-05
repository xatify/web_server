#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include "CharStreamer.hpp"

class Token {
	private:
		std::string		token;
		unsigned int	ln;
		unsigned int	clmn;
	public:
		Token (std::string tkn = "", u_int32_t _line = 0, u_int32_t _column = 0);
		bool empty () const;
		void clear ();
		void append (const CharStreamer&);
		std::string status (const char *) const;
		u_int32_t& line (void);
		u_int32_t& column (void);
		const std::string& id () const;
		void expect (const std::string&);
};

#endif