#ifndef __CONNECTIONHANDLER_HPP_
#define __CONNECTIONHANDLER_HPP_

#include <vector>
#include <map>

class Connection;
class ListenConnection;

class ConnectionsHandler {
	private:
		std::vector <struct pollfd>		pollfds;
		std::map <int, Connection *>	cons;
		bool setUp ();
		bool addListenConnection (ListenConnection *con);
		ConnectionsHandler (const ConnectionsHandler&);
		ConnectionsHandler& operator = (const ConnectionsHandler&);
	public:
		ConnectionsHandler ();
		~ConnectionsHandler ();
		void start ();
};

#endif