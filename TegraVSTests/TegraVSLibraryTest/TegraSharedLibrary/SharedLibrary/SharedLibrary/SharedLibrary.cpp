#include "SharedLibrary.h"

/* This trivial function returns the platform ABI for which this dynamic native library is compiled.*/
const char * SharedLibrary::GetPlatformABI()
{
	return "Hello from Shared Library";
}

void SharedLibrary()
{
}

SharedLibrary::SharedLibrary()
{
}

SharedLibrary::~SharedLibrary()
{
}

