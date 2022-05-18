#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <poll.h>

enum  E {
	READ = POLLIN,
	WRITE = POLLOUT
};

class Event {
	private:
		int event;
	public:
		Event	(int e): event (e & (READ | WRITE)) {}
		~Event	(){}
		operator bool () const { return event; }
		bool operator == (int e) const { return (event & e); }
};

#endif