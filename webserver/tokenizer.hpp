#ifndef __TOKENIZER_HPP_
#define __TOKENIZER_HPP_

#include "tokens.hpp"
#include <exception>
#include <vector>

class Tokenizer {
	private:
		std::vector <Token> tokens;
		std::vector<Token>::iterator current;
	public:
		struct Error: public std::exception  {
			std::string error;
			Error (const std::string& _error);
			~Error () _NOEXCEPT {}
			virtual const char *what () const _NOEXCEPT;
		};
		Tokenizer (const char *file);
		~Tokenizer ();
		void expect (const std::string& id);
		Token operator *() const;
		Tokenizer& operator++ ();
		operator bool ();
};

#endif