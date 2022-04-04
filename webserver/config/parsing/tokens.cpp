
#include "tokens.hpp"

Token::Token (std::string _token = "", u_int32_t _line = 0, u_int32_t _column): token (_token), ln (_line), clmn (_column) {}

u_int32_t Token::column (void) const { return clmn; }

u_int32_t Token::line (void) const { return ln; }
