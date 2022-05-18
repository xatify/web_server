#include "ListenConnection.hpp"

#include "Connection.hpp"
#include "DataConnection.hpp"
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h>


ListenConnection::ListenConnection (int fd): Connection (fd, "listen") {}

ListenConnection::~ListenConnection () {}

bool ListenConnection::accept (std::map <int, Connection *>& cons, std::vector<struct pollfd>& polls) {
	struct pollfd pollstrct;
	bool accepted = false;
	int	new_fd;

	do {
		new_fd = ::accept (this->getFd (), 0x0, 0x0);
		if (new_fd < 0) break;
		// set non blocking mode on new_fd
		int flags = fcntl (new_fd, F_GETFL);
		flags |= O_NONBLOCK;
		fcntl (new_fd, F_SETFL, flags);
		accepted = true;
		cons.insert (std::make_pair(new_fd, new dataConnection (new_fd)));
		pollstrct.fd = new_fd;
		pollstrct.events = POLLIN | POLLOUT;
		polls.push_back (pollstrct);
	}while (1);
	return accepted;
}

