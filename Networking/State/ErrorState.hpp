#ifndef __ERROR_STATE_HPP__
#define __ERROR_STATE_HPP__

#include "State.hpp"

class ErrorState: public State {
	public:
		ErrorState (dataConnection *_context = 0x0);
		void handle ();
};

#endif