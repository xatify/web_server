#include "DataConnection.hpp"

#include <unistd.h>
#include "Connection.hpp"
#include "../Event/Event.hpp"
#include "../Activity/Activity.hpp"
#include "../../Request/Request.hpp"
#include "../../Response/Response.hpp"
#include "../../ServerConfiguration/Configuration.hpp"


dataConnection::dataConnection (int fd): Connection (fd, "data") {
    Configuration::instance ()->getLogger ().AccessLog (*this);
    activity    = new Activity ();
    request     = new Request ();
    response    = new Response ();
}

void dataConnection::execute (Event e) {
	if (e == READ) {
		size_t size = 1025;
		char *buff = new char [size];
		size_t rd = ::read (this->getFd (), buff, size);
		if (rd <= 0) {
			delete [] buff;
			this->setState (CLOSE);
		}
		input.append (buff, buff + rd);
	}
	//if (e == WRITE)
	// 	output.write ();
	
	// if (input.isclosed () || output.isclosed ())
	// 	state = CLOSE;
	switch (this->getState ()) {
		PARSING:
			if (!request->isParsed ()) {
				std::string::size_type n = input.find ("\r\n");
				if (n != std::string::npos) {
					if (request->parse (input.substr (0, n)) == false) {
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
				this->setState (EXECUTE);
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