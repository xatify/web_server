
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main () {
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof (address);

	char *hello = "HTTP/1.1 200 OK\r\nContent-Type: Text/plain\r\nContent-Length: 12\r\n\r\nHello world!";

	if ((server_fd = socket (AF_INET, SOCK_STREAM, 0)) == 0) {
		perror ("In socket");
		exit (1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons (PORT);

	memset (address.sin_zero, 0, sizeof (address.sin_zero));

	if (bind (server_fd, (struct sockaddr *)&address, sizeof (address)) < 0) {
		perror ("In bind");
		exit (1);
	}

	if (listen (server_fd, 10) < 0) {
		perror ("In listen");
		exit (1);
	}

	while (1) {
		printf ("+++++++++++ waiting for connection +++++++++++++++++\n");
		if ((new_socket = accept (server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror ("listen");
			exit (1);
		}

		char buffer[30000] = {0};

		valread = read (new_socket, buffer, 30000);
		printf ("%s\n", buffer);
		write (new_socket, hello, strlen (hello));
		
		printf ("----- hello sent-------\n");
		close (new_socket);
	}
}
