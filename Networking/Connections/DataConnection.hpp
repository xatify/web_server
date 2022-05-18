#ifndef __DATA_CONNECTION_HPP__
#define __DATA_CONNECTION_HPP__

#include <string>

class Connection;
class Event;
class Request;
class Activity;
class Request;
class Response;

class dataConnection: public Connection {
    private:
        std::string     input;
        Activity        *activity;
        Request         *request;
        Response        *response;
    public:
        dataConnection (int fd);
        ~dataConnection ();
        void execute (Event e);
};
#endif