#ifndef __CIRCULLAR_BUFFER_HPP__
#define __CIRCULLAR_BUFFER_HPP__

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
		Buffer (size_t	sz = 1024): _sz (sz), full (false) {
			start = new char [_sz];
			rptr = wptr = start;
			end  = start + _sz;
		}
		~Buffer () { delete [] start; }
		void clear () { rptr = wptr; }
		bool isclosed () const {}
};

class InputBuffer: public Buffer {
	public:
		InputBuffer (size_t sz): Buffer (sz) {}
		size_t	empty_space (void) {
			if (wptr == end) wptr = start;
			if (wptr > rptr)
				return end - wptr;
			ptrdiff_t size = rptr - wptr;
			if (!full && size == 0) return _sz;
			return size;
		}
		size_t	full_space (void) {
			if (rptr == end) rptr == start;
			if (rptr > wptr)
				return end - rptr;
			ptrdiff_t size = wptr - rptr;
			if (full && size == 0) return _sz;
			return size;
		}
		bool	import_ (int fd) {
			if (full) return true;
			size_t size = empty_space ();
			ssize_t rd = ::read (fd, wptr, size);
			if (rd <= 0) return false;
			wptr += rd;
			if (wptr == rptr) full = true;
			return true;
		}
		// char	*export_ (size_t& size) {
		// 	size = full_space ();
		// 	return  ptr;
		// 	if (empty_space == 0)
		// 		size = 0;
		// 	}
		// }
};
#endif