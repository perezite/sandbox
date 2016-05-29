#include <iostream>

#include <boost/coroutine/asymmetric_coroutine.hpp>
using namespace boost::coroutines;

void inner(coroutine<void>::push_type& yield)
{
	std::cout << "inner 1" << std::endl;
	yield();

	std::cout << "inner 2" << std::endl;
}

void outer(coroutine<void>::push_type& yield)
{
	std::cout << "outer 1 " << std::endl;
	yield();

	coroutine<void>::pull_type gen { inner };
	yield();
	while (gen)
	{
		gen();
	}
	yield();
	 
	std::cout << "outer 3" << std::endl;
}

int main()
{
	coroutine<void>::pull_type gen { outer };
	std::cout << "main" << std::endl;
	while (gen)
	{
		gen();
		std::cout << "main" << std::endl;
	}

	std::cin.get();

	return 0;
}