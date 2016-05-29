#include <iostream>
#include <vector>

#include <boost/coroutine/coroutine.hpp>
using namespace boost::coroutines;

void(*g_outerCoroutine)(push_coroutine<void>&);

void StartCoroutine(void(*runner)(push_coroutine<void>&))
{
	// g_outerCoroutine = new pull_coroutine<void>{ runner };
	g_outerCoroutine = runner;
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
		std::cout << "inner " << i << std::endl;
		yield();
	}

	std::cout << "inner " << max << std::endl;
}

void outer(push_coroutine<void>& yield)
{
	std::cout << "outer 1" << std::endl;
	yield();

	StartInnerCoroutine(inner, yield);

	std::cout << "outer 2" << std::endl;
}

void run()
{
	pull_coroutine<void> gen { g_outerCoroutine };
	std::cout << "main" << std::endl;

	while (gen)
	{
		gen();
		std::cout << "main" << std::endl;
	}
}

int main()
{
	StartCoroutine(outer);

	run();

	std::cout << "done" << std::endl;

	std::cin.get();
}
