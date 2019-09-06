#include "Demo1.h"
#include "Demo2.h"
#include <iostream>

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	demo2::run();

	return 0;
}
