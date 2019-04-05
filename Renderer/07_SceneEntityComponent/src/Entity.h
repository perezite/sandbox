#pragma once 
#include <string>


namespace sb 
{
	class Entity
	{
	public:
		Entity(std::string name)
			: m_name (name)
		{ }
	private:
		std::string m_name;
	};
}