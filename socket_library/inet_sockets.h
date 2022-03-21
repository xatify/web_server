
#ifndef INET_SOCKET_H
#define INET_SOCKET_H

#include <sys/socket.h>
#include <netdb.h>

int inet_Connet (const char *host, const char *service, int type);

int inetListen (const char *service, int backlog, socklen_t *addrlen);

int inetBind (const char *service, int type, socklen_t *addrlen);

char *inetAddressStr (const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrlen);

#define IS_ADDR_STR_LEN 4098

#endif

