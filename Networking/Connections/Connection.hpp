#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <string>

class Connection {
	private:
		std::string		type;
		int				fd;
	public:
		Connection (int fd, const std::string& _type);
		virtual  ~Connection ();
		const	std::string& getType () const;
		int		getFd () const;
		void	close ();
};

#endif