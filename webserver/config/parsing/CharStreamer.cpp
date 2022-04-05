
#include "CharStreamer.hpp"

CharStreamer::CharStreamer (const char *file): ln (0), clmn (0), sz (0){
	if (fb.open (file, std::ios::in)) {
		std::istream is (&fb);
		std::noskipws (is);
		it = std::istream_iterator <char> (is);
	}
	else
		throw std::invalid_argument ("couldn't open file");
}

CharStreamer::~CharStreamer () { fb.close (); }

char CharStreamer::operator * () const {
	if (empty ()) return 0x0;
	return *it; 
}

bool CharStreamer::empty () const { return it == end; }

bool CharStreamer::operator == (const char c) const {
	if (empty ()) return false;
	return (*it == c);
}

bool CharStreamer::operator != (const char c) const {
	return ! (*this == c);
}

void CharStreamer::next () { 
	if (it != end) {
		++it; 
		sz++;
		clmn++;
		if (*it == '\n') {
			ln++;
			clmn = 0;
		}
	}
}








