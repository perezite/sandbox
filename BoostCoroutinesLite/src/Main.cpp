#include <iostream>
#include <vector>

#include <boost/coroutine/coroutine.hpp>
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
		std::cout << "code in INNER coroutine, call " << i << std::endl;
		yield();
	}

	std::cout << "finishing INNER coroutine" << std::endl;
}

void outer(push_coroutine<void>& yield)
{
	std::cout << "code in OUTER coroutine, call 1" << std::endl;
	yield();

	std::cout << "OUTER coroutine calling INNER coroutine" << std::endl;
	StartInnerCoroutine(inner, yield);

	std::cout << "finishing OUTER coroutine" << std::endl;
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
}

void CoroutineWaitingForOtherCoroutineDemo()
{
	StartCoroutine(outer);

	run();

	std::cout << "done" << std::endl;

	std::cin.get();
}

int main()
{
	CoroutineWaitingForOtherCoroutineDemo();
}

