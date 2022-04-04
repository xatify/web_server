#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "../configuration/configuration.hpp"

#include <exception>
#include "tokenizer.hpp"

class Parser: public Tokenizer {
	public:
		Parser (const char *file);
		HttpConfig 	parse_http () const;
		Server 		parse_server () const;
		Location	parse_location() const;
		std::vector <std::string> parse_server_name () const;
		Listen parse_listen () const;
		u_int32_t parse_body_size () const;
		// parse_error_page ();
		// parse_auto_index ();
};

#endif