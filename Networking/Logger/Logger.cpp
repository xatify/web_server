#include "Logger.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../Connections/Connection.hpp"

Logger::Logger (const std::string& error, const std::string& access) {
	errorfd		= open (error.c_str (), O_WRONLY | O_CREAT | O_TRUNC);
	accessfd	=  open (access.c_str (), O_WRONLY | O_CREAT | O_TRUNC);
}

Logger::~Logger () {
	close (errorfd);
	close (accessfd);
}

void Logger::ErrorLog (const std::string& err) const {
	if (errorfd >= 0)
		write (errorfd, err.data (), err.size ());
}

void Logger::AccessLog (const std::string& err) const{
	if (accessfd >= 0)
		write (errorfd, err.data (), err.size ());
}

void Logger::AccessLog (const Connection& con) const {
	if (accessfd >= 0) {
		struct		sockaddr_in address;
		socklen_t	address_len = sizeof (address);
		int res = getpeername(con.getFd (), (struct sockaddr *)&address, &address_len);
		std::string log = "New Connection: ";
		log += inet_ntoa(address.sin_addr);
		log += ":";
		log += ntohs(address.sin_port);
		log += "\n";
		AccessLog (log);
	}
}

