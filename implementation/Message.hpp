#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>
#include <map>
#include "Event.hpp"

class RequestLine {
	private:
		std::string		method;
		std::string		URL;
		std::string		httpversion;
		bool			isparsed;
	public:
		RequestLine (): isparsed (false) {}
		~RequestLine ();
		
		void setMethod (const std::string& m) { method = m; }
		const std::string& getMethod () const { return method; }
		
		const std::string& getURL () const { return URL; }
		void setURL (const std::string& url) { URL = url; }

		const std::string& getVersion () const { return httpversion; }
		void setVersion (const std::string& v) { httpversion = v; }
		
		bool isParsed () const { return isparsed; }
		bool parse (const std::string&);
		void reset ();
};

class HeaderFields {
	private:
		std::map <std::string, std::vector<std::string> > hf;
		bool isparsed;
	public:
		HeaderFields (): isparsed(false) {}
		~HeaderFields () {}
		bool parse (const std::string&);
		bool isParsed () const { return isparsed; }		
};

class Request {
	private:
		RequestLine		requestline;
		HeaderFields	hf;
		// Payload pl;
	public:
		Request ();
		~Request ();
		bool isParsed () const {
			return requestline.isParsed() && hf.isParsed ();
		}
		bool parse (const std::string&);
};

#endif