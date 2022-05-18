#ifndef __ACTIVITY_HPP__
#define __ACTIVITY_HPP__

#include <sys/time.h>

class Activity {
	private:
		struct timeval time; 
	public:
		Activity ();
		~Activity ();
		bool timeOut () const;
		void reset ();
};


#endif