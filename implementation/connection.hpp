#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <vector>
#include <map>
#include "Circullar_Buffer.hpp"
#include "Message.hpp"
#include <poll.h>
#include <string>

enum State {
			PARSING,
			ERROR,
			SENDING,
			EXECUTE,
			CLOSE
			};
 

class Event {
	private:
		int event;
	public:
		Event	(int e): event (e & (POLLIN | POLLOUT)) {}
		~Event	(){}
		operator bool () const { return event; }
		bool operator == (int e) const { return (event & e); }
};

class Connection {
	protected:
		std::string		type;
		int				fd;
		State			state;
	public:
		Connection (int _fd, const std::string& _type): fd (_fd), type (_type) {}
		const std::string& getType () const;
		void setState (State st) { state = st; }
		int getFd () const { return fd; }
		const std::string& getType () const { return type; }
		State getState () const { return state; }
		virtual ~Connection () {}
};

class ListenConnection: public Connection {
	public:
		ListenConnection (int fd);
		~ListenConnection () {}
		bool listen(std::map<int, Connection*>&, std::vector<struct pollfd>&);
};


class dataConnection: public Connection {
	private:
		Activity		active;
		//Activity		active;		// storing the last time 
									// event of read has been 
									// declared on the connection
		// Request			rq;
		// Response		rs;
		Buffer			input;
		Buffer			output;
	public:
		dataConnection (int fd);
		void execute (Event e); //{
		// 	switch (state) {
	
		// 	}
		// }
};


class ConnectionsHandler {
	private:
		std::vector <struct pollfd>		pollfds;
		std::map <int, Connection *>	cons;
		//Logger 		logger;
	public:
		ConnectionsHandler ();
		~ConnectionsHandler ();
		bool addListenConnection (ListenConnection *con);
		void start ();
};



class Activity {

};

#endif