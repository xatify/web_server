#include "Message.hpp"
#include <sstream>
#include <vector>

bool RequestLine::isParsed () const  {
	return !method.empty () && !URL.empty () && !httpversion.empty ();
}

void RequestLine::reset (void) {
	// clear method, URL, httpversion
}

bool RequestLine::parse (const std::string& input) {	
	// split input with whitespaces
	// if the number of splits != 3 return false
	// else
	// method = splits[0]
	// URL  = splits[1]
	// httpversion = splits[2]
	// isparsed = true;
	//return true
	return true;
}

bool HeaderFields::parse (const std::string& input) {
	// if (input is empty) {
	// isparsed = true;
	// return true;
	//}
	// parse the input string :
	// parse the key 
	// key.tolowercase 
	// parse the value
	// hf[key] = value;
	// if someerror happend return false
	// else
	// return true;
}

Request::Request () {
	////
	////
}
Request::~Request () {
	///
	///
}


bool Request::parse (const std::string& input) {
	if (!requestline.isParsed ())
		return requestline.parse (input);
	if (!hf.isParsed ())
		return hf.parse (input);
}