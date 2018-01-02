#include "Behaviour.h"

#include <iostream>

bool State::Run()
{	
	if (m_status == 0)
	{
		Enter();
		m_status = 1;
	}
	else if (m_status == 1)
	{
		bool running = Update();
		if (running == false)
			m_status = 2;
	}
	else if (m_status == 2)
	{
		Exit();
		m_status = 3;
	}
	
	return m_status != 3;
}
