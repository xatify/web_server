#ifndef __TOKENIZER_HPP_
#define __TOKENIZER_HPP_

#include <exception>
#include <vector>
#include <fstream>

class Token;

struct Error: public std::exception  {
	std::string error;
	Error (const std::string& _error);
	~Error () _NOEXCEPT {}
	virtual const char *what () const _NOEXCEPT;
};

class Tokenizer {
	private:
		std::vector <const Token> tokens;
		const std::string file;
		bool _ready;
		std::vector<const Token>::const_iterator current;
		void add_token (std::string&, unsigned int, unsigned int);
	public:
		Tokenizer (const char  *file);
		~Tokenizer ();
		std::string error (const char *error) const;
		void tokenize ();
		bool ready () const;
		const std::string& id () const;
		void print_tokens () const;
		void expect (const std::string& id);
		const Token& operator *() const;
		Tokenizer& operator++ ();
		operator bool ();
};

#endif