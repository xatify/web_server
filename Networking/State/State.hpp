#ifndef __STATE_HPP__
#define __STATE_HPP__


class dataConnection;

class State {
	protected:
		dataConnection	*context;
    public:
		State (dataConnection *_context): context (_context) {}
		void setContext (dataConnection *_context) {
			this->context = _context;
		} 
        virtual void handle () = 0;
};

#endif