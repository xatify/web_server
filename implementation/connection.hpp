#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <cstddef>
#include <unistd.h>

// read from a fd
// and the process can extract information

class Buffer {
	protected:
		char	*start;
		char	*end;
		char	*rptr;
		char	*wptr;
		size_t	_sz;
		bool	full;
	public:
		Buffer (size_t	sz): _sz (), full (false) {
			start = new char [_sz];
			rptr = wptr = start;
			end  = start + _sz;
		}
		~Buffer () { delete [] start; }
		void clear () { rptr = wptr; }

};
class InputBuffer: public Buffer {
	public:
		size_t	available_forwrite (void) {
			ptrdiff_t size = wptr - rptr;
			return (size < 0)? size + _sz: size; 
		}
		size_t	available_toread (void) {
			return _sz - available_forwrite ();
		}

		bool read (int fd) {
			if (full) return false;
			
		}

};

class Connection {
};
#endif