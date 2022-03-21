#define _BSD_SOURCE

#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "inet_sockets.h"

int inetConnect (const char *host, const char *service, int type) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;

	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = type;

	s = getaddrinfo (host, service, &hints, &result);
	if (s != 0) {
		errno = ENOSYS;
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue ;
		if (connect (sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break ;
		close (sfd);
	}

	freeaddrinfo (result);

	return (rp == NULL) ? -1: sfd;
}

static int inetPassiveSocket (const char *service, int type, socklen_t *addrlen, int doListen, int backlog) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	int sfd, optval, s;

	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	s = getaddrinfo (NULL, service, &hints, &result);
	if (s != 0)
		return -1;
	
	optval = 1;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue ;
		
		if (doListen) {
			if (setsockopt (sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)) == -1) {
				close (sfd);
				freeaddrinfo (result);
				return -1;
			}
		}

		if (bind (sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break ;
		
		close (sfd);
	}

	if (rp != NULL && doListen) {
		if (listen (sfd, backlog) == -1) {
			freeaddrinfo (result);
			return -1;
		}
	}

	if (rp != NULL && addrlen != NULL)
		*addrlen = rp->ai_addrlen;
	freeaddrinfo (result);

	return (rp == NULL) ? -1: sfd;
}

int inetListen (const char *service, int backlog, socklen_t *addrlen) {
	return inetPassiveSocket (service, SOCK_STREAM, addrlen, 1, backlog);
}

int inetBind (const char *service, int type, socklen_t *addrlen) {
	return inetPassiveSocket (service, type, addrlen, 0, 0);
}

char *inetAddressStr (const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrlen) {
	char host [NI_MAXHOST], service [NI_MAXSERV];

	if (getnameinfo (addr, addrlen, host, NI_MAXHOST, serice, NI_MAXSERV) == 0)
		snprintf (addrStr, addrStrlen, "(%s, %s)", host, service);
	else
		snprintf (addrStr, addrStrlen, "(?UNKNOWN?)");
	
	addStr [addStr - 1] = '\0';
	return addrStr;
}

