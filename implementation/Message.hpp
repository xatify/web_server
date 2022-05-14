#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__


class Message {
	public:
		void reset ();
}

class Request: public Message;
class Response: public Message;

#endif