#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <arpa/inet.h>

class Addr {
	private:
		sockaddr_in addr_;
	public:
		Addr (unsigned short port, unsigned int addr = 0);
		unsigned short getPort () const;
		unsigned int getIpAddr () const;
		const sockaddr *addr () const;
		size_t size () const;
};


class ListenSocket {
	protected:
		int		fd;
	public:
		ListenSocket (int);
		ListenSocket ();
		int getFd () const;
		bool bind (const Addr&, int backlog = 15);
		void close ();
};


#endif