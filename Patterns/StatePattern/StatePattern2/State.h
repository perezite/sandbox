#pragma once

#include "Behaviour.h"
#include <iostream>

class Behaviour;

class State
{	
public:
	State(Behaviour* parent) : 
		m_parent(parent), m_status(0)
	{
	}

	bool Run();
	
	virtual void Enter() { };
	
	virtual bool Update() { return false; }
	
	virtual void Exit() { };
	
	Behaviour* GetParent() { return m_parent; }
	
	int GetStatus() { return m_status; }
	
	void SetName(std::string name) { m_name = name; }
	
	std::string GetName() { return m_name; }
	
private:
	Behaviour* m_parent;

	int m_status;
	
	std::string m_name;
	
};
