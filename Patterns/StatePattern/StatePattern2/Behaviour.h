#pragma once

#include "State.h"

class Behaviour
{	
public:
	Behaviour() : m_state(NULL), m_queuedState(NULL) {}

	virtual ~Behaviour();
	
	void UpdateState();
	
	void Update();
	
	void QueueState(State* state) { m_queuedState = state; } 
	
	State* m_state;

private:

	State* m_queuedState;
};
