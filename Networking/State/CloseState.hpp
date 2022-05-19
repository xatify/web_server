#ifndef __CLOSE_STATE_HPP__
#define __CLOSE_STATE_HPP__

#include "State.hpp"

class CloseState: public State {
	public:
		CloseState (dataConnection *_context = 0x0);
		void handle ();
};

#endif
