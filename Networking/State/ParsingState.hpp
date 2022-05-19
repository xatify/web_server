#ifndef __PARSING_STATE_HPP__
#define __PARSING_STATE_HPP__

#include "State.hpp"

class ParsingState: public State {
	public:
		ParsingState (dataConnection *_context = 0x0);
		void handle ();
};

#endif