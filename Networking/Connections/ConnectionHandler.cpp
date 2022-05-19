#include "ConnectionHandler.hpp"

#include <set>
#include <poll.h>
#include "Connection.hpp"
#include "ListenConnection.hpp"
#include "DataConnection.hpp"
#include "../Sockets/socket.hpp"
#include "../../ServerConfiguration/Configuration.hpp"
#include "../Event/Event.hpp"

ConnectionsHandler::ConnectionsHandler () {}

ConnectionsHandler::~ConnectionsHandler () {
    for (std::map<int, Connection *>::iterator it = cons.begin (); it != cons.end (); ++it) {
        (*it).second->close ();
        delete ((*it).second);
    }
}

bool ConnectionsHandler::addListenConnection (ListenConnection *con) {
    struct pollfd poll;
	if (con) {
		if (cons.find (con->getFd ()) == cons.end ()) {
			cons.insert (std::make_pair(con->getFd (), con));
			poll.fd = con->getFd ();
			poll.events = POLLIN;
			pollfds.push_back (poll);
			return true;
		}
	}
	return false;
}

bool ConnectionsHandler::setUp () {
    std::set<std::pair<u_int32_t, u_int16_t> > listens;

	std::vector<Component *> Vservers (Configuration::instance ()->getConfigurations ()->getSubComponents ("server"));
	for (std::vector<Component *>::iterator it = Vservers.begin (); it != Vservers.end (); ++it) {
		Listen		*listen = dynamic_cast <Listen *> ((*it)->getSimpleAttribute ("listen"));
		listens.insert (std::make_pair (listen->getAddress (), listen->getPort ()));
	}
	std::vector <ListenConnection *> listenCons;
	bool error = false;
	for (std::set<std::pair<u_int32_t, u_int16_t> >::const_iterator it = listens.begin (); it != listens.end (); ++it) {
		Addr 			addr ((*it).second, (*it).first);
		ListenSocket	sock;
		if (sock.getFd () < 0) {
			// log error canot create socket 
			error = true;
			break;
		}
		if (!sock.bind (addr)) {
			sock.close ();
			// log error cannot bind to Address:Port
			error = true;
			break;
		}
		listenCons.push_back (new ListenConnection (sock.getFd ()));
	}
	for (std::vector<ListenConnection *>::iterator it = listenCons.begin (); it != listenCons.end (); ++it) {
		if (error) delete (*it);
		else
			addListenConnection (*it);
	}
	return !error;
}

void ConnectionsHandler::start () {
	if (this->setUp ()) {
		std::vector <struct pollfd> 		newpolls;
		do {
			pollfds.insert (pollfds.end (), newpolls.begin (), newpolls.end ());
			newpolls.clear();
			int ret = ::poll (&(pollfds[0]), pollfds.size (), -1);
			for (std::vector<struct pollfd>::iterator it = pollfds.begin (); it != pollfds.end (); ++it) {
				Event event (it->revents);
				if (!event) {
					if ()
					//  using Activity class of the connection
					// if it is a dataConnection and it timeouted close it
					// and log the close event
					continue;
				}
				
				Connection *connection = cons.find (it->fd)->second;
				if (connection->getType () == "listen") {
					ListenConnection *Listen = dynamic_cast<ListenConnection*>(connection);
					if (Listen) {
						if (Listen->accept (cons, newpolls) == false) {
							//error shutdow the server
							//return;
						}
					}
				}
				else {
					dataConnection *Data = dynamic_cast <dataConnection*> (connection);
					Data->handle (event);
				}
			}
		}while (true);
	}
}
