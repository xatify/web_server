
#include "socket.hpp"
#include <iostream>
#include <poll.h>
#include <map>
#include <fcntl.h>
#include <unistd.h>

#define SERVER_PORT1 12345
#define SERVER_PORT2 8080

int main () {
	int rc;
	bool stop = false;
	char buffer[1024];
	
	Addr addr1 (SERVER_PORT1);
	Addr addr2 (SERVER_PORT2);

	ListenSocket L1;
	ListenSocket L2;

	if (!L1.bind (addr1)) {
		std::cout << "cannot bind to SERVER_PORT1\n";
		return 1; 
	}
	if (!L2.bind (addr1)) {
		std::cout << "cannot bind to SERVER_PORT2\n";
		return 1; 
	}

	bool end_server = false;
	struct pollfd fds[200];
	int nfds = 2;

	memset (fds, 0, sizeof fds);
	fds[0].fd = L1.getFd ();
	fds[0].fd = POLLIN;

	fds[1].fd = L2.getFd ();
	fds[1].fd = POLLIN;

#define LISTEN 0
#define STREAM 1

	std::map <int, int> socket_type;

	socket_type.insert (std::make_pair (L1.getFd (), LISTEN));
	socket_type.insert (std::make_pair (L2.getFd (), LISTEN));
	while (end_server == false) {
		std::cerr << "Waiting on poll () ...\n";
		rc = poll (fds, nfds, 0);
		if (rc < 0) {
			std::cerr << "poll  () failed\n";
			return 1;
		}
		int current_size = nfds;
		for (int i = 0; i < current_size; i++) {
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents & POLLIN) {
				if (socket_type.at(fds[i].fd) == LISTEN) {
					std::cerr << "	listening socket is readable\n";
					do {
						int new_fd = accept (fds[i].fd, 0x0, 0x0);
						if (new_fd < 0) {
							if (errno != EWOULDBLOCK) {
								std::cerr << "	accept () failed";
								stop = true;
							}
							break;
						}
						fcntl (new_fd, F_SETFL, O_NONBLOCK);
						std::cerr << "	New incoming connection - " << new_fd << "\n";
						fds[nfds].fd = new_fd;
						fds[nfds].events = POLLIN;
						nfds++;
						socket_type[new_fd] = STREAM;
					}
					while (true);
				}
				else {
					std::cerr << "	Descriptor " << fds[i].fd << " is readable\n";
					rc = read (fds[i].fd, buffer, sizeof buffer - 1);
					if (rc == 0)
						

				}
			}
		}
	}

}