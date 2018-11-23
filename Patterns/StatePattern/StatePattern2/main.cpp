#include "ExplodingBehaviour.h"
#include "FreestyleBehaviour.h"

#include <iostream>
#include <stdio.h>
#in
/*
int main()
{
	ExplodingBehaviour* behaviour = new ExplodingBehaviour();
	FreestyleBehavior* behaviour2 = new FreestyleBehavior();

	while (true)
	{
		std::cout << "frame start" << std::endl;

			std::cout << "	regular behaviour start" << std::endl;
			behaviour->UpdateState();
			behaviour->Update();
			std::cout << "	regular behaviour end" << std::endl;

			std::cout << "	freestye behaviour start" << std::endl;
			behaviour2->Update();
			std::cout << "	freestye behaviour end" << std::endl;

		std::cout << "frame end" << std::endl;
		getchar();
 	}
	
	delete behaviour;
	
	return 0;
}

*/

template <class I, class V>
struct persistent
{
	persistent(I* instance_, std::string name_, V initial) {

	}

	void Init()
	{
		instance = instance_;
		name = name;
		value = initial;
	}

	const persistent& operator=(const V& other) {
		value = other;
	}

	const persistent& operator=(const persistent<V, I>& other) {
		value = other.value;
	}

	std::setbuf

	V value;

	I* instance;

	std::string name;
};

class Tester
{
	void Test()
	{
		persistent<Tester, int> counter(this, "counter", 42);
	}
};

int main()
{
	return 0;
}