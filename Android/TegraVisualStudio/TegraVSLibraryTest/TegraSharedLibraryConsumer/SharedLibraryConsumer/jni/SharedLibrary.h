#pragma once

class SharedLibrary
{
public:
	const char * GetPlatformABI();
	SharedLibrary();
	~SharedLibrary();
};

