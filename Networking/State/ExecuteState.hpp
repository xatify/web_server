#ifndef __EXECUTE_STATE_HPP__
#define __EXECUTE_STATE_HPP__

#include "State.hpp"
class ExecuteState: public State {
	public:
		ExecuteState (dataConnection *_context = 0x0);
		void handle ();
};

#endif