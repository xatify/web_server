#include "Connection.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../Sockets/socket.hpp"
#include <set>

ListenConnection::ListenConnection (int fd): Connection (fd, "listen") {}

bool ListenConnection::listen (std::map<int, Connection*>& connections, std::vector<struct pollfd>& polls) {
	struct pollfd pollstrct;
	bool accepted = false;
	int	new_fd;

	do {
		new_fd = ::accept (fd, 0x0, 0x0);
		if (new_fd < 0) break;
		// set non blocking mode on new_fd
		int flags = fcntl (new_fd, F_GETFL);
		flags |= O_NONBLOCK;
		fcntl (new_fd, F_SETFL, flags);
		accepted = true;
		connections.insert (std::make_pair(new_fd, new dataConnection (new_fd)));
		pollstrct.fd = new_fd;
		pollstrct.events = POLLIN | POLLOUT;
		polls.push_back (pollstrct);
	}while (1);
	return accepted;
}

dataConnection::dataConnection (int fd): Connection (fd, "data") {
	Configuration::instance ()->getLogger ().AccessLog (*this);
}

ConnectionsHandler::ConnectionsHandler () {
	//initaite
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
			error = true;
			break;
		}
		if (!sock.bind (addr)) {
			sock.close ();
			error = true;
			break;
		}
	}
	for (std::vector<ListenConnection *>::iterator it = listenCons.begin (); it != listenCons.end (); ++it) {
		if (error) delete (*it);
		else
			addListenConnection (*it);
	}
	return !error;
}

void ConnectionsHandler::start () {
	if (setUp ()) {
		std::vector <struct pollfd> 		newpolls;
		if (cons.empty ()) {
			Configuration::instance ()->getLogger ().AccessLog ("no connection to listen to\n");
			return;
		}
		do {
			pollfds.insert (pollfds.end (), newpolls.begin (), newpolls.end ());
			newpolls.clear();
			int ret = ::poll (&(pollfds[0]), pollfds.size (), -1);
			for (std::vector<struct pollfd>::iterator it = pollfds.begin (); it != pollfds.end (); ++it) {
				
				Event event (it->revents);
				if (!event) {
					//  using Activity class of the connection
					// if it is a dataConnection and it timeouted close it
					// and log the close event
					continue;
				}
				
				Connection *connection = cons.find (it->fd)->second;
				if (connection->getType () == "listen") {
					ListenConnection *Listen = dynamic_cast<ListenConnection*>(connection);
					if (Listen) {
						if (Listen->listen (cons, newpolls) == false) {
							//error shutdow the server
							//return;
						}
					}
				}
				else {
					dataConnection *Data = dynamic_cast <dataConnection*> (connection);
					Data->execute (event);
				}
			}
		}while (true);
	}
}


void dataConnection::execute (Event e) {
	if (e == READ) {
		size_t size = 1025;
		char *buff = new char [size];
		size_t rd = ::read (getFd (), buff, size);
		if (rd <= 0) {
			delete [] buff;
			state = CLOSE;
		}
		input.append (buff, buff + rd);
	}
	//if (e == WRITE)
	// 	output.write ();
	
	// if (input.isclosed () || output.isclosed ())
	// 	state = CLOSE;
	switch (state) {
		PARSING:
			if (!rq.isParsed ()) {
				std::string::size_type n = input.find ("\r\n");
				if (n != std::string::npos) {
					if (rq.parse (input.substr (0, n)) == false) {
						// state = ERROR
						
					}
					else {
						input.assign (input.begin () + n + 2, input.end ());
					}
				}
				// else if (input.size () > SOMEMAXVALUE)
				// 		state = ERROR
			}
			else
				state = EXECUTE;
		ERROR:
			// generate appropriate Response
			// state = SENDING;
			// break;
		EXECUTE:
			// depending on the request
			// if payload is expected and request is not complete yet
			//		rq.recvpaylod ();
			// apply URL mapping 
			//	- redirection
			//		generate redirection response
			//	- static file
			//		generate static file response
			// 	- cgi
			//		generate cgi
			// if enykind of error happens here 
			// set state to error
			// and generate appropriate response
		SENDING:
			// sending response 
			//  through output buffer
		CLOSE:
	}
}

