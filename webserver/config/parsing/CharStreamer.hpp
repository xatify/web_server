#ifndef __CHAR_STREAMER_HPP_
#define __CHAR_STREAMER_HPP_

#include <fstream>

class CharStreamer {
	private:
		const std::istream_iterator <char>	end;
		std::istream_iterator <char>		it;
		std::filebuf						fb;
		unsigned int						ln;
		unsigned int						clmn;
		unsigned int						sz;
	public:
		CharStreamer (const char *file);
		~CharStreamer ();
		char operator * () const;
		bool empty () const;
		void next ();
		bool operator == (const char) const;
		bool operator != (const char) const;
		unsigned int line () const { return ln; }
		unsigned int column () const { return clmn; }
		unsigned int size () const { return sz; }

};

#endif