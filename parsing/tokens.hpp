#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include <iostream>

class Token {
	private:
		std::string		token;
		unsigned int	ln;
		unsigned int	clmn;
		static const char * directives [];
	public:
		Token (std::string tkn = "", unsigned int _line = 0, unsigned int _column = 0);
		Token (const Token& tkn);
		Token& operator = (const Token& rhs);
		~Token () {};
		bool empty () const;
		std::string status (const char *) const;
		bool is_directive () const;
		u_int32_t& line (void);
		u_int32_t& column (void);
		const std::string& id () const;
		bool isNumber () const;
		void expect (const std::string&) const;
		friend std::ostream& operator << (std::ostream&  output, const Token &token);
};

#endif