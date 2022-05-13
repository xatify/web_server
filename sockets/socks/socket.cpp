#include "socket.hpp"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

Addr::Addr (unsigned short port, unsigned int addr) {
	memset (&addr_, 0, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons (port);
	addr_.sin_addr.s_addr = htonl (addr);
}

unsigned short Addr::getPort () const  { return addr_.sin_port; }

unsigned int Addr::getIpAddr () const  { return addr_.sin_addr.s_addr; }

const sockaddr *Addr::addr () const { return reinterpret_cast <const sockaddr *> (&(addr_)); }

size_t Addr::size () const { return sizeof addr_; }


Socket::Socket (int _fd): fd(_fd) {}

Socket::Socket () {
	int fd = socket (AF_INET, SOCK_STREAM, 0);
}

int Socket::getFd () const { return fd; }

void Socket::close () { ::close (fd); }


ListenSocket::ListenSocket (): Socket () {
	int on = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
	fcntl (fd, F_SETFL, O_NONBLOCK);
}

bool ListenSocket::bind (const Addr& addr_, int backlog) {
	int rc = ::bind (fd, addr_.addr (), addr_.size ());
	if (rc < 0) {
		close ();
		return false;
	}
	rc = ::listen (fd, backlog);
	if (rc < 0) {
		close ();
		return false;
	}
	return true;
}
