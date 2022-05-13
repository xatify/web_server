#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <errno.h>

#define SERVER_PORT 12345

#define TRUE	1
#define FALSE	0

int main () {
	int len, rc, on = 1;
	int listen_sd = -1, new_sd = -1;
	int end_server = FALSE, compress_array = FALSE;
	int close_conn;
	char buffer[80];
	struct sockaddr_in6 addr;
	int timeout;
	struct pollfd fds[200];
	int nfds = 1, current_size = 0, i, j;
	const char resp[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\ncontent-length: 10\r\n\r\nhellothere";

	listen_sd = socket (AF_INET6, SOCK_STREAM, 0);
	if (listen_sd < 0) {
		perror ("socket () failed");
		exit (-1);
	}

	rc = setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
	if (rc < 0) {
		perror ("setsockopt () failed");
		close (listen_sd);
		exit (-1);
	}

	rc = fcntl (listen_sd, F_SETFL, O_NONBLOCK);
	if (rc < 0) {
		perror ("iostl () failed");
		close (listen_sd);
		exit (-1);
	}

	memset (&addr, 0, sizeof addr);
	addr.sin6_family = AF_INET6;
	memcpy (&addr.sin6_addr, &in6addr_any, sizeof (in6addr_any));
	addr.sin6_port = htons (SERVER_PORT);
	rc = bind (listen_sd, (struct sockaddr *)&addr, sizeof (addr));
	if (rc < 0) {
		perror ("bind () failed");
		close (listen_sd);
		exit (-1);
	}

	rc = listen (listen_sd, 32);
	if (rc < 0) {
		perror ("listen () failed");
		close (listen_sd);
		exit (-1);
	}

	memset (fds, 0, sizeof (fds));

	fds[0].fd = listen_sd;
	fds[0].events = POLLIN;


	

	timeout = (3 * 60 * 1000);
	do {
		printf (" current fds = %d\n", nfds);
		printf ("Waiting on poll () ...\n");
		rc = poll (fds, nfds, -1);
		if (rc < 0) {
			perror ("  poll () failed");
			break;
		}
		if (rc == 0) {
			printf ("  poll timed out. End program.\n");
			break ;
		}
		current_size = nfds;
		for (i = 0; i < current_size; i++) {
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == listen_sd) {
					printf ("	listening socket is readable\n");
					do {
						new_sd = accept (listen_sd, 0x0, 0x0);
						if (new_sd < 0) {
							if (errno != EWOULDBLOCK) {
								perror ("	accept () failed");
								end_server = TRUE;
							}
							break;
						}
						fcntl (new_sd, F_SETFL, O_NONBLOCK);
						printf ("	New incoming connection - %d\n", new_sd);
						fds[nfds].fd = new_sd;
						fds[nfds].events = POLLIN;
						nfds++;
					}while (new_sd != -1);
				}
				else  {
					printf ("	Descriptor %d is readable\n", fds[i].fd);
					close_conn = FALSE;
					rc = read (fds[i].fd, buffer, sizeof (buffer));
					if (rc == 0)
						close_conn = TRUE;
					else  {
						do {
							len = rc;
							printf ("	%d bytes received\n", len);
							rc = read (fds[i].fd, buffer, sizeof buffer);
							if (rc < 0) {
								if (errno != EWOULDBLOCK) {
									perror ("	recv () failed\n");
									close_conn = TRUE;
								}
								break;
							}
							if (rc == 0) {
								break;
							}
							len = rc;
							printf ("	%d bytes received\n", len);
						} while (TRUE);
						fds[i].events |= POLLOUT;
					}
					if (close_conn == TRUE) {
						close (fds[i].fd);
						fds[i].fd = -1;
						compress_array = TRUE;
						continue;
					}
				}
			}
			else if (fds[i].revents & POLLOUT) {
				rc = send (fds[i].fd, resp, sizeof resp, 0);
				if (rc < 0) {
					perror ("	send () failed");
					close (fds[i].fd);
					fds[i].fd = -1;
					compress_array = TRUE;
					continue;
				}
				fds[i].events &= (~POLLOUT);
				continue;

			}
			else {
				printf ("	error! revents = %d\n", fds[i].revents);
				end_server = TRUE;
				break;
			}
		}
		if (compress_array) {
			compress_array = FALSE;
			for (i = 0; i < nfds; i++) {
				if (fds[i].fd == -1) {
					for (j = i; j < nfds; j++) {
						fds[j] = fds[j + 1];
					}
					i--;
					nfds--;
				}
			}
		}
	}
	while (end_server == FALSE);
	for (i = 0; i < nfds; i++) {
		if (fds[i].fd >= 0)
			close (fds[i].fd);
	}

}