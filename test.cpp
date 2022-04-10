
#include <iostream>
#include <arpa/inet.h>

int main () {
	u_int32_t add = 45;

	struct in_addr addr;

	addr.s_addr = add;

	std::cout << inet_ntoa (addr);

}