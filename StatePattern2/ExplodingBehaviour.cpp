#include "ExplodingBehaviour.h"

#include <iostream>

ExplodingBehaviour::ExplodingBehaviour() : Behaviour()
{
	m_state = new WaitingState(this);
}

void ExplodingBehaviour::Update()
{	
	std::cout << "		" << m_state->GetName().c_str() << " " << m_state->GetStatus() << std::endl;
	if (m_state->Run())
	{
	}
}

void WaitingState::Enter()
{
	std::cout << "		enter waiting " << std::endl;;
}
	
bool WaitingState::Update()
{
	std::cout << "		do waiting " << m_counter << std::endl;  

	return false;
}

void WaitingState::Exit()
{
	std::cout << "		exit waiting" << std::endl;
	auto state = new ExplodingState((Behaviour*)this);
	GetParent()->QueueState(state);
}

void ExplodingState::Enter()
{
	std::cout << "		boom" << std::endl;
}