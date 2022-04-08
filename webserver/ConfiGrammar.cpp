
#include "ConfiGrammar.hpp"

Component::Component (const std::string& _directive, bool one, bool after, bool brackets) {
	directive = _directive;
	only_one = one;
	after_directive = after;
	bracketed = brackets;
}

void Component::syntax_parse (Tokenizer& tokenizer) {
	if ((*tokenizer).id () == directive) {
		if (only_one) {
			if (isSet ()) // tokenizer error log should be more specific
				Tokenizer::Error ((*tokenizer).status ("only one attribute allowed"));
		}
		++tokenizer;
		if (after_directive) {
			if ((*tokenizer).is_directive ())
				throw Tokenizer::Error ((*tokenizer).status ("expected token"));
			af_directive = (*tokenizer).id ();
			++tokenizer;
		}
		if (bracketed)
			tokenizer.expect ("{");
		parse (tokenizer);
		if (!isSet ())
			Tokenizer::Error ((*tokenizer).status ("expected token"));
		if (bracketed)
			tokenizer.expect ("}");
		else
			tokenizer.expect (";");
	}
}

