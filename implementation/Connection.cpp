#include "Connection.hpp"
#include "sys/socket.h"
#include <fcntl.h>

ListenConnection::ListenConnection (int fd): Connection (fd) {}

std::vector<dataConnection *> ListenConnection::listen (void) {
	std::vector <dataConnection *> connections;
	int	new_fd;
	do {
		new_fd = ::accept (fd, 0x0, 0x0);
		if (new_fd < 0) break;
		fcntl (new_fd, F_SETFL, O_NONBLOCK);
		connections.push_back (new dataConnection (new_fd));
	}while (1);
	return connections;
}

dataConnection::dataConnection (int fd): Connection (fd) {
	// handling of set start time
	// 
}

void dataConnection::execute (Event e) {
	switch (e) {
		READ:
			input.read ();
			break;
		WRITE:
			output.write ();
	}
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

