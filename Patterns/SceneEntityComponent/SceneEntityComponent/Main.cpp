#include "Demo1.h"
#include <iostream>

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	demo1::run();

	return 0;
}
