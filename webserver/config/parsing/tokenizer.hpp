#ifndef __TOKENIZER_HPP_
#define __TOKENIZER_HPP_

#include "tokens.hpp"
#include "CharStreamer.hpp"
#include <exception>

class Tokenizer: public Token {
	private:
		CharStreamer  stream;
	private:
		void skip_comment ();
	public:
		struct Error: public std::exception  {
			std::string error;
			Error (const std::string& _error);
			virtual const char *what () const _NOEXCEPT;
		};
		Tokenizer (const char *file);
		~Tokenizer ();
		Token next_token ();
		Token current_token () const;
		void expect (const std::string& id);
		operator bool () const;
};

#endif