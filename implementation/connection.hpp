#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <vector>
#include <map>
#include "Buffer.hpp"
#include "Message.hpp"
#include <string>
#include "Activity.hpp"
#include "Event.hpp"

enum State {
			PARSING,
			ERROR,
			SENDING,
			EXECUTE,
			CLOSE
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
		std::string		input;
		// Buffer			input;
		// Buffer			output;
		Activity		active;
		Request		rq;
		// Response		rs;
	public:
		dataConnection (int fd);
		void execute (Event e);
};


class ConnectionsHandler {
	private:
		std::vector <struct pollfd>		pollfds;
		std::map <int, Connection *>	cons;
	public:
		ConnectionsHandler ();
		~ConnectionsHandler ();
		bool addListenConnection (ListenConnection *con);
		void start ();
};


#endif