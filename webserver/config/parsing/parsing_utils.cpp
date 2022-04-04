
#include <string>
#include <arpa/inet.h>


bool to_integer (const std::string token, int* holder = 0x0) {
	int 	hldr = 0;
	std::string::const_iterator it = token.begin ();
	while ((it != token.end ()) && isdigit (*it)) {
		hldr = (hldr * 10) + (*it - '0');
		++it;
	}
	if (it != token.end () && !isdigit (*it)) return false;
	*holder = hldr;
	return true;
}

bool is_port (const std::string token, unsigned short *prt = 0x0) {
	int	 port;

	if (to_integer (token, &port)) {
		if (port >= 0 && port < 65535) {
			if (prt) *prt = port;
			return true;
		}
	}
	return false;
}

bool is_address (const std::string token, u_int32_t *address = 0x0) {
	uint32_t addr;

	if (token.find (".") == std::string::npos)
		return false;
	if (inet_pton (AF_INET, token.c_str (), &addr) == 1) {
		if (address) *address = addr;
		return true;
	}
	return false;
}
