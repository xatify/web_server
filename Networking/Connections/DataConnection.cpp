#include "DataConnection.hpp"

#include <unistd.h>
#include "Connection.hpp"
#include "../Event/Event.hpp"
#include "../Activity/Activity.hpp"
#include "../../Request/Request.hpp"
#include "../../Response/Response.hpp"
#include "../../ServerConfiguration/Configuration.hpp"
#include "../State/State.hpp"
#include "../State/ParsingState.hpp"


dataConnection::dataConnection (int fd): Connection (fd, "data") {
    Configuration::instance ()->getLogger ().AccessLog (*this);
    activity    = new Activity ();
    request     = new Request ();
    response    = new Response ();
	state		= nullptr;
	close		= false;
	this->TranslationTo(new ParsingState ());
}

dataConnection::~dataConnection () {
	delete activity;
	delete request;
	delete response;
	delete state;
}

Request *dataConnection::getRequest () {
	return request;
}

bool dataConnection::isClose () const {
	return close;
}

void dataConnection::SetState (State *_state) {
	this->state = _state;
}

std::string& dataConnection::getInput () {
	return input;
}

const Event& dataConnection::getEvent () const {
	return event;
}

bool& dataConnection::getClose () {
	return close;
}

void dataConnection::TranslationTo (State *_state) {
	if (this->state != nullptr)
		delete state;
	this->state = state;
	this->state->setContext (this);
}

void dataConnection::handle (Event e) {
	event	= e;
	state->handle ();
}

	// 	PARSING:
	// 		
	// 	ERROR:
	// 		// generate appropriate Response
	// 		// state = SENDING;
	// 		// break;
	// 	EXECUTE:
	// 		// depending on the request
	// 		// if payload is expected and request is not complete yet
	// 		//		rq.recvpaylod ();
	// 		// apply URL mapping 
	// 		//	- redirection
	// 		//		generate redirection response
	// 		//	- static file
	// 		//		generate static file response
	// 		// 	- cgi
	// 		//		generate cgi
	// 		// if enykind of error happens here 
	// 		// set state to error
	// 		// and generate appropriate response
	// 	SENDING:
	// 		// sending response 
	// 		//  through output buffer
	// 	CLOSE:
	// }