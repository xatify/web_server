#ifndef __LISTEN_CONNECTION_HPP__
#define __LISTEN_CONNECTION_HPP__

class Connection;
#include <map>
#include <vector>

class ListenConnection: public Connection {
    public:
        ListenConnection (int fd);
        ~ListenConnection ();
        bool accept (std::map <int, Connection *>&, std::vector<struct pollfd>&);
};

#endif