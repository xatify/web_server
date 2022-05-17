#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <cstddef>
#include <string>
#include <string.h>
#include <unistd.h>

// class Buffer {
// 	private:
// 		char		*buff;
// 		size_t		capacity;
// 		size_t		size;
// 		void reallocate () {
// 			capacity = capacity * 2;
// 			char *tmp = new char [capacity * 2];
// 			::memcpy (tmp, buff, size);
// 			delete [] buff;
// 			buff = tmp;
// 		}
// 	public:
// 		Buffer (size_t _cap = 1024): capacity (_cap), size (0) {
// 			buff = new char [capacity];
// 		}
// 		~Buffer () { delete [] buff; }
		
// 		void clear () { size = 0; }

// 		size_t size () const { return size; }
// 		size_t capacity () const  { return capacity; }

// 		bool operator << (int fd) {
// 			if (size == capacity) {
// 				this->reallocate ();
// 			}
// 			ssize_t rd = ::read (fd, buff + size, capacity - size);
// 			if (rd <= 0) return false;
// 			size += rd;
// 			return true;
// 		}

// 		bool getData (std::string& str, size_t _sz) {
// 			if (_sz > size) return false;
// 			str.append (buff, buff + size);
// 			::memcpy (buff, buff + size, size - _sz);
// 			size -= _sz;
// 			return true;
// 		}
// 		bool search (const std::string&) {
// 			if ()
// 		}

// };


#endif