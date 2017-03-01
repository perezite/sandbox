#include "Settings.h"

#include <iostream>
#include <string>
#include <exception>

std::string ToString(bool value);

bool ToBool(std::string value);

int main()
{
	// show current value
	std::cout << "Current value for IsSoundEnabled: " << ToString(Settings::GetIsSoundEnabled()) << std::endl;

	// get new value
	std::string isSoundEnabled;
	std::cout << "Set new value (s to skip, 0 for false, 1 for true): ";
	std::cin >> isSoundEnabled;
	
	// store new value
	bool skipSettingNewValue = (isSoundEnabled == "s");	
	if (!skipSettingNewValue)
	{
		bool value = ToBool(isSoundEnabled);
		Settings::SetIsSoundEnabled(value);
	}
		
	// show new value
	if (!skipSettingNewValue)
	{
		std::cout << "New value for IsSoundEnabled: " << ToString(Settings::GetIsSoundEnabled()) << std::endl;		
	}

	return 0;
}

std::string ToString(bool value)
{
	return value == true ? "true" : "false"; 
}

bool ToBool(std::string value)
{
	if (value == "0")
		return false;
	else if (value == "1")
		return true;
	else
		throw std::exception();
}