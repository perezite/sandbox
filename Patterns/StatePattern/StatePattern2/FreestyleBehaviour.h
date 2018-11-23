#pragma once

#include "Behaviour.h"

class FreestyleBehavior : public Behaviour
{
enum State
{
	Waiting = 0,
	Exploding = 1,
	Idle = 2
};

public:
	FreestyleBehavior() : m_state(State::Waiting) { }

	void Update();

private:
	State m_state;
};