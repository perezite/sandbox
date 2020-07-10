#include "Demo.h"
#include "Logger.h"

#include <iostream>
#include <random>

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Audio - Build: " << configuration << ", " << __DATE__ << ", " << __TIME__);
}

int main() {
	version();

	demo::run();

	return 0;
}
