#include <iostream>
#include <vector>

#include <boost/coroutine/coroutine.hpp>
#include "Main.h"
using namespace boost::coroutines;

std::vector<void(*)(push_coroutine<void>&)> g_outerCoroutines;
std::vector<bool> g_outerCoroutinesActiveatable;

void StartCoroutine(void(*runner)(push_coroutine<void>&))
{
	g_outerCoroutines.push_back(runner);
	g_outerCoroutinesActiveatable.push_back(true);
}

void StartInnerCoroutine(void(*runner)(push_coroutine<void>&), push_coroutine<void>& yield)
{
	pull_coroutine<void> gen{ runner };
	yield();

	while (gen)
	{
		gen();
		yield();
	}
}

void inner(push_coroutine<void>& yield)
{
	int max = 2;
	for (int i = 0; i < max; i++)
	{
		std::cout << "code in INNER, call " << i << std::endl;
		yield();
	}
}

void outer(push_coroutine<void>& yield)
{
	std::cout << "code in OUTER, call 0" << std::endl;
	yield();

	std::cout << "OUTER calling INNER" << std::endl;
	StartInnerCoroutine(inner, yield);

	std::cout << "code in OUTER, call 1" << std::endl;
}

void outer2(push_coroutine<void>& yield)
{
	for (int i = 0; i < 3; i++)
	{
		std::cout << "code in OUTER2, call " << i << std::endl;
		yield();
	}
}

void outer3(push_coroutine<void>& yield)
{
	for (int i = 0; i < 3; i++)
	{
		std::cout << "code in OUTER3, call " << i << std::endl;
		yield();
	}
}

void outer4(push_coroutine<void>& yield)
{
	std::cout << "code in OUTER4, call 0" << std::endl;
	yield();

	std::cout << "OUTER4 spawning OUTER3" << std::endl;
	StartCoroutine(outer3);
	yield();

	for (int i = 1; i < 4; i++)
	{
		std::cout << "code in OUTER4, call " << i << std::endl;
		yield();
	}
}

void run()
{
	std::vector<pull_coroutine<void>*> coroutines;
	bool running = true;

	while (running)
	{
		bool hasActiveCoroutines = false;

		// execute the active coroutines
		for each (pull_coroutine<void>* coroutine in coroutines)
		{
			if (*coroutine)
			{
				(*coroutine)();
				hasActiveCoroutines = true;
			}
		}

		// activate activeatable coroutines
		for (unsigned int i = 0; i < g_outerCoroutinesActiveatable.size(); i++)
		{
			if (g_outerCoroutinesActiveatable[i] == true)
			{
				pull_coroutine<void> *coroutine = new pull_coroutine<void>{ g_outerCoroutines[i] };
				coroutines.push_back(coroutine);
				g_outerCoroutinesActiveatable[i] = false;
				hasActiveCoroutines = true;
			}
		}

		running = hasActiveCoroutines;
	}

	// cleanup 
	for each (pull_coroutine<void>* coroutine in coroutines)
	{
		delete coroutine;
	}
	g_outerCoroutines.clear();
	g_outerCoroutinesActiveatable.clear();
}

void CoroutineWaitingForAnotherCoroutineDemo()
{
	std::cout << "Demo: Coroutine waiting for another coroutine" << std::endl;

	StartCoroutine(outer);

	run();

	std::cout << "done" << std::endl;

	std::cin.get();
}

void TwoConcurrentCoroutinesDemo()
{
	std::cout << "Demo: Two concurrent coroutines demo" << std::endl;

	StartCoroutine(outer2);
	StartCoroutine(outer3);

	run();
	std::cout << "done" << std::endl;

	std::cin.get();
}

void CoroutineSpawningAnotherCoroutineDemo()
{
	std::cout << "Demo: Coroutine spawning another independent coroutine" << std::endl;

	StartCoroutine(outer4);

	run();
	std::cout << "done" << std::endl;
	std::cin.get();
}

int main()
{
	CoroutineWaitingForAnotherCoroutineDemo();

	TwoConcurrentCoroutinesDemo();

	CoroutineSpawningAnotherCoroutineDemo();
}

