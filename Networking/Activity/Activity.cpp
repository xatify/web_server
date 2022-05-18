#include "Activity.hpp"
#include "../../ServerConfiguration/Configuration.hpp"

Activity::Activity () {
	this->reset ();	
}

bool Activity::timeOut () const {
	struct timeval now;
	gettimeofday (& now, 0x0);
	now.tv_sec += Configuration::instance ()->keepAlive ();
	return !(time < now);
}

void Activity::reset () { gettimeofday (&time, 0x0); }

bool operator < (const struct timeval& t1, const struct timeval& t2) {
	if (t1.tv_sec < t2.tv_sec) return true;
	if (t1.tv_sec == t2.tv_sec && t1.tv_usec <= t2.tv_usec) return true;
	return false;
}
