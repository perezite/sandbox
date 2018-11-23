#pragma once

#include "Behaviour.h"

#include <iostream>

class ExplodingBehaviour : public Behaviour
{	
public:
	ExplodingBehaviour();
	
	void Update();
};

class WaitingState : public State
{	
public:
	WaitingState(Behaviour* parent) 
		: m_counter(0), State(parent)
	{
		SetName("Waiting State");
	}		

	void Enter();
	
	bool Update();
	
	void Exit();
	
private:
	unsigned int m_counter;
};

class ExplodingState : public State
{	
public:
	ExplodingState(Behaviour* parent) 
		: State(parent)
	{
		SetName("Exploding State");
	}		

	void Enter();
};