#ifndef __DATA_CONNECTION_HPP__
#define __DATA_CONNECTION_HPP__

#include <string>

#include "Connection.hpp"
#include "../Event/Event.hpp"

class Request;
class Activity;
class Request;
class Response;
class State;


class dataConnection: public Connection {
    private:
        std::string     input;
        Activity        *activity;
        Request         *request;
        Response        *response;
        State           *state;
        Event           event;
        bool            close;
    public:
        dataConnection (int fd);
        ~dataConnection ();
        std::string& getInput ();
        Request *getRequest ();
        void TranslationTo (State *);
        const Event& getEvent () const;
        bool& getClose ();
        void SetState (State *);
        bool isClose () const;
        void handle (Event e);
};
#endif