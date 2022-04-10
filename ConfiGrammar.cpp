
#include "ConfiGrammar.hpp"

Component::Component (const std::string& _directive, bool one, bool after, bool brackets) {
	directive = _directive;
	only_one = one;
	after_directive = after;
	bracketed = brackets;
}

void Component::syntax_parse (Tokenizer& tokenizer) {
	if (tokenizer.id () == directive) {
		if (only_one) {
			if (isSet ()) // tokenizer error log should be more specific
				throw Error (tokenizer.error ("multiple directive, same name"));
		}
		++tokenizer;
		if (after_directive) {
			if ((*tokenizer).is_directive ())
				throw Error (tokenizer.error ("expected token"));
			af_directive = (*tokenizer).id ();
			++tokenizer;
		}
		if (bracketed)
			tokenizer.expect ("{");
		parse (tokenizer);
		if (!isSet ())
			throw Error (tokenizer.error ("empty directive"));
		if (bracketed)
			tokenizer.expect ("}");
		else
			tokenizer.expect (";");
		if (directive == "http" && (tokenizer))
			throw Error (tokenizer.error ("http block already closed"));
		
	}
	if (tokenizer && !(*tokenizer).is_directive ())
		throw Error (tokenizer.error ("directive unknown"));
}

void Component::pretty_print (std::string tabulation) const {
	if (isSet ()) {
		std::cout << tabulation << directive << " ";
		if (after_directive)
			std::cout << af_directive << " ";
		if (bracketed)
			std::cout << "{\n";
		print (tabulation + "\t");
		if (bracketed)
			std::cout << tabulation << "}\n";
		else
			std::cout << ";\n";
	}
}
