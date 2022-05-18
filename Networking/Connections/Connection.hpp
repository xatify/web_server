#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <string>

enum State {
	PARSING,
	ERROR,
	SENDING,
	EXECUTE,
	CLOSE
};

class Connection {
	private:
		std::string		type;
		int				fd;
		State			state;
	public:
		Connection (int fd, const std::string& _type);
		virtual  ~Connection ();
		const	std::string& getType () const;
		int		getFd () const;
		State	getState () const;
		void	setState (State e);
		void	close ();
};

#endif