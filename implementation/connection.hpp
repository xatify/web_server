#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <vector>
#include <map>
#include "Circullar_Buffer.hpp"
#include "Message.hpp"

enum State {
			PARSING,
			ERROR,
			SENDING,
			EXECUTE,
			CLOSE
			};

enum Event {
			READ,
			WRITE
			};

class Connection {
	protected:
		int			fd;
		State		state;
	public:
		Connection (int _fd): fd (_fd) {}
		void setState (State st) { state = st; }
		int getFd () const { return fd; }
		State getState () const { return state; }
		virtual ~Connection () {}
		virtual void execute (Event e);
};

class ListenConnection: public Connection {
	public:
		ListenConnection (int fd);
		std::vector<dataConnection*> listen();
};


class dataConnection: public Connection {
	private:
		//Activity		active;		// storing the last time 
									// event of read has been 
									// declared on the connection
		Request			rq;
		Response		rs;
		Buffer			input;
		Buffer			output;
	public:
		dataConnection (int fd);
		void execute (Event e); //{
		// 	switch (state) {
	
		// 	}
		// }
};

class Connection_handler {
	private:
		//std::vector <
		std::map <int, Connection *> Connections;
	public:
		Connection_handler () {}
		void add_Listenconnection (ListenConnection x) {};
		void start () {
		}
};


#endif