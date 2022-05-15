#include "Connection.hpp"
#include <sys/socket.h>
#include <fcntl.h>

ListenConnection::ListenConnection (int fd): Connection (fd, "listen") {}

bool ListenConnection::listen (std::map<int, Connection*>& connections, std::vector<struct pollfd>& polls) {
	struct pollfd pollstrct;
	bool accepted = false;
	int	new_fd;

	do {
		new_fd = ::accept (fd, 0x0, 0x0);
		if (new_fd < 0) break;
		//fcntl (new_fd, F_SETFL, O_NONBLOCK);
		accepted = true;
		connections.insert (std::make_pair(new_fd, new dataConnection (new_fd)));
		pollstrct.fd = new_fd;
		pollstrct.events = POLLIN | POLLOUT;
		polls.push_back (pollstrct);
	}while (1);
	return accepted;
}

dataConnection::dataConnection (int fd): Connection (fd, "data") {
	// handling of set start time
	// 
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

void ConnectionsHandler::start () {
std::vector <struct pollfd> 		newpolls;
	if (cons.empty ()) {
		// log: no listen connections;
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


void dataConnection::execute (Event e) {
	if (e == POLLIN)
		input.read ();
	if (e == POLLOUT)
		output.write ();
	
	if (input.isclosed () || output.isclosed ())
		state = CLOSE;
	switch (state) {
		PARSING:
			if (rq.parse (input) < 0) // if there is error in parsing
				state = ERROR;
			if (rq.isdone ())
				state = EXECUTE;
			break;
		ERROR:
			// generate appropriate Response
			state = SENDING;
			break;
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

