
#include <string>
#include <iostream>

int main () {
	std::string i = "hello world";

	i.assign (i.begin () + 3, i.end ());
	std::cout << i;
}