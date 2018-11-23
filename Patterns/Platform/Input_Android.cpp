#include "Input.h"

#ifdef ANDROID 

std::string Input::GetTouch()
{
	return "GetTouch() successful!";
}

std::string Input::GetMouse()
{
	return "GetMouse() is not supported on android";	
}

#endif
