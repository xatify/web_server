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


ListenSocket::ListenSocket () {
	fd = socket (AF_INET, SOCK_STREAM, 0);
	if (fd < 0) return;
	int on = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
	int flags = fcntl (fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl (fd, F_SETFL, flags);
}

int ListenSocket::getFd () const { return fd; }

void ListenSocket::close () { ::close (fd); }


ListenSocket::ListenSocket () {
	
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
