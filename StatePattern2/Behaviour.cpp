#include "Behaviour.h"

#include <iostream>

Behaviour::~Behaviour()
{
	if (m_state != NULL)
		delete m_state;
}

void Behaviour::UpdateState()
{
	if (m_queuedState != NULL)
	{
		if (m_state)
			delete m_state;
		m_state = m_queuedState;
		m_queuedState = NULL;
	}

}

void Behaviour::Update()
{	
	if (m_state->Run())
	{}
}
