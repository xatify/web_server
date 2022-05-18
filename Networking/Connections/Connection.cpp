#include "Connection.hpp"

#include <unistd.h>

Connection::Connection (int _fd, const std::string& _type): fd (_fd), type (_type) {}
Connection::~Connection () {}
const std::string& Connection::getType () const { return type; }
int	Connection::getFd () const { return fd; }
State	Connection::getState () const { return state; }
void	Connection::setState (State e) { state = e; }
void	Connection::close () { ::close (fd); }
