#include <iostream>
#include <windows.h>

using namespace std;

void hello();

int main()
{
	hello();

	return 0;
}

void hello()
{
	std::cout << "Hello World" << std::endl;

	system("pause");
}

