#include "Settings.h"

#include <fstream>
#include <iostream>

bool Settings::m_isSoundEnabled = false;

const std::string Settings::m_storageFileName = "settings.txt";

bool Settings::GetIsSoundEnabled()
{
	m_isSoundEnabled = LoadValue(m_isSoundEnabled);
	return m_isSoundEnabled;
}
	
void Settings::SetIsSoundEnabled(bool isEnabled)
{
	m_isSoundEnabled = isEnabled;
	SaveValue(m_isSoundEnabled);
}

bool Settings::LoadValue(bool defaultValue)
{
	std::string line;
	std::ifstream storageFile(m_storageFileName.c_str());
	
	if (storageFile.is_open())
	{
		getline (storageFile, line);
		storageFile.close();
	}
	
	if (line.empty())
		line = ToString(defaultValue);
	
	return ToBool(line);
}

void Settings::SaveValue(bool isEnabled)
{
	std::ofstream storageFile;
	storageFile.open(m_storageFileName.c_str(), std::ios::out | std::ios::trunc);
	
	if(storageFile.is_open())
	{
		storageFile << ToString(isEnabled);
		storageFile.close();
	}
}

std::string Settings::ToString(bool value)
{
	return value == true ? "true" : "false"; 
}

bool Settings::ToBool(std::string value)
{
	if (value == "false")
		return false;
	else if (value == "true")
		return true;
	else
		throw std::exception();
}
