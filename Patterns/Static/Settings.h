#pragma once

#include <string>

class Settings
{
public:
	// get is sound enabled
	static bool GetIsSoundEnabled();
	
	// set is sound enabled
	static void SetIsSoundEnabled(bool isEnabled);
	
protected:
	// load value from file
	static bool LoadValue(bool defaultValue);
	
	// save value to file
	static void SaveValue(bool isEnabled);
	
	// convert string to bool
	static bool ToBool(std::string value);
	
	// convert bool to string
	static std::string ToString(bool value);
	
private:
	// storage file name
	static const std::string m_storageFileName;
	
	// is sound enabled
	static bool m_isSoundEnabled;
	
};