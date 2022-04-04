#ifndef __TOKENIZER_HPP_
#define __TOKENIZER_HPP_

#include "tokens.hpp"
#include <exception>
#include <fstream>


class Tokenizer {
	private:
		unsigned int line;
		unsigned int column;
		const std::istream_iterator <char> end;
		std::istream_iterator <char> it;
		std::filebuf				fb;
		std::string					token;
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

};
#endif