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


class Socket {
	protected:
		int		fd;
	public:
		Socket (int);
		Socket ();
		int getFd () const;
		void close ();
};


class ListenSocket: public Socket {
	public:
		ListenSocket ();
		bool bind (const Addr&, int backlog = 15);
};



#endif