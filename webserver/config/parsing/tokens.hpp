#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>

class Token {
	private:
		std::string	token;
		u_int32_t	ln;
		u_int32_t	clmn;
	public:
		Token (std::string tkn, u_int32_t _line = 0, u_int32_t _column = 0);
		u_int32_t line (void) const;
		u_int32_t column (void) const;
};

#endif