/* // Thats the way I would like it to be
void coroutine_test();

int main()
{
	coroutine_test();
	coroutine_test();
	
	// expected output: 42 \n 43
	
	return 0;
}

void coroutine_test()
{
	int test = 42;
	
	std::cout << test << std::endl;
	
	yield();
	
	int test = 43;
	
	std::cout << test << std::endl;
}*/

#include <iostream>
#include <string>
#include <map>

void coroutine_test();

template <class T> struct coroutineVariable
{
	std::string name;
	
	T value;
};

class Coroutine
{
	public:
		Coroutine() : m_section(0) { }		
	
		void setInteger(std::string name, int value) { m_integers[name] = value; }
		
		int getInteger(std::string name) { return m_integers[name]; }
	
		int getSection() { return m_section; }
	
		void yield() { m_section++; }
	
	private:
		std::map<std::string, int> m_integers;
		
		unsigned int m_section;
};

#define CROW_YIELD() 		\
	{ 						\
			coro.yield(); 	\
			return; 		\
		} 					\
	} 

Coroutine coro;

int main()
{
	coroutine_test();
	coroutine_test();
}

void coroutine_test()
{	
	if (coro.getSection() == 0)
	{
		coro.setInteger("test", 42);
	
		std::cout << "section 1 " << coro.getInteger("test") << std::endl;
	
		CROW_YIELD()
	
	else if (coro.getSection() == 1)
	{
		int test = coro.getInteger("test");
		coro.setInteger("test", ++test);
	
		std::cout << "section 2 " << coro.getInteger("test") << std::endl;	
		return;
	}
}

