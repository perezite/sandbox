#include "Input.h"

#ifdef WIN32 

std::string Input::GetTouch()
{
	return "GetTouch() is not supported on windows";
}

std::string Input::GetMouse()
{
	return "GetMouse() successful!";
}

#endif
