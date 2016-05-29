#include <iostream>
#include <vector>

#include <boost/coroutine/asymmetric_coroutine.hpp>
using namespace boost::coroutines;

void StartCoroutine(void(*runner)(push_coroutine<void>&))
{
	pull_coroutine<void> gen{ runner };

	while (gen)
	{
		gen();
	}
}

void simpler(push_coroutine<void>& yield)
{
	std::cout << "simpler 1" << std::endl;
	yield();

	std::cout << "simpler 2" << std::endl;
}

void simple(push_coroutine<void>& yield)
{
	std::cout << "simple 1" << std::endl;
	yield();

	StartCoroutine(simpler);

	std::cout << "simple 2" << std::endl;
}

int main()
{
	StartCoroutine(simple);
}
