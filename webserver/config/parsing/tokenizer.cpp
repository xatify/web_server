#include "tokenizer.hpp"


Tokenizer::Error::Error (const std::string& _error): error (_error) {}

const char* Tokenizer::Error::what () const _NOEXCEPT { return error.c_str (); }


Tokenizer::Tokenizer (const char *file): stream (file) {}

Tokenizer::~Tokenizer () {}

Token Tokenizer::next_token() {
	this->clear ();
	while (!stream.empty ()) {
		if (stream == '#') { // skip comments
			while (stream != '\n') stream.next ();
			stream.next ();
		}
		if (isspace (*stream) && !empty ()) break;
		if (stream == ';') {
			if (empty ()) {
				append (stream);
				stream.next ();
			}
			break;
		}
		append (stream);
		stream.next ();
 	}
	return *this;
}


Token Tokenizer::current_token () const {
	return *this;
}

void Tokenizer::expect (const std::string& id) {
	next_token ().expect (id);
}

Tokenizer::operator bool () const {
	return current_token ().empty ();
}