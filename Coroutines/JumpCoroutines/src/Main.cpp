#include <iostream>

#include <setjmp.h>
#include <stdlib.h>

struct CoContext
{
	void *vars;

	template <class T>
	void createVars()
	{
		vars = new T();
	}

	template <class T>
	T* getVars()
	{
		return (T*)vars;
	}
};

CoContext coContext;
CoContext co2Context;
jmp_buf Main, Coro, Coro2;
bool CoroYielded = true, Coro2Yielded = true;
int iter = 0;

struct CoroResult
{
	CoroResult(bool& val)
	{
		val = false;
		longjmp(Main, 1);
	}
};

CoroResult coro()
{
	struct Variables
	{
		int i = 0;
	};
	coContext.createVars<Variables>();
	Variables *ctx = coContext.getVars<Variables>();

	for (ctx->i = 0; ctx->i < 4; ctx->i++) {
		std::cout << ctx->i << " Coro" << std::endl;
		if (setjmp(Coro) == 0) {
			longjmp(Main, 1);
		}
		else
			ctx = coContext.getVars<Variables>();
	}

	delete ctx;
	return CoroResult(CoroYielded);
}

CoroResult coro2()
{
	struct Variables
	{
		char dummy = 0;
	};
	co2Context.createVars<Variables>();
	Variables *ctx = co2Context.getVars<Variables>();

	std::cout << "Coro2 1" << std::endl;

	if (setjmp(Coro2) == 0) {
		longjmp(Main, 1);
	} else
		ctx = co2Context.getVars<Variables>();


	std::cout << "Coro2 2" << std::endl;
	if (setjmp(Coro2) == 0) {
		longjmp(Main, 1);
	} else
		ctx = co2Context.getVars<Variables>();

	std::cout << "Coro2 3" << std::endl;
	if (setjmp(Coro2) == 0) {
		longjmp(Main, 1);
	} else
		ctx = co2Context.getVars<Variables>();

	delete ctx;
	return CoroResult(Coro2Yielded);
}

void corotest()
{
	// coro
	if (setjmp(Main) == 0)
		coro();

	while (CoroYielded)
	{
		std::cout << "Main" << std::endl;
		if (setjmp(Main) == 0)
			longjmp(Coro, 1);
	}

	// coro2
	if (setjmp(Main) == 0)
		coro2();

	while (Coro2Yielded)
	{
		std::cout << "Main" << std::endl;
		if (setjmp(Main) == 0)
			longjmp(Coro2, 1);
	}

	std::cin.get();
}

int main(int argc, char* args[])
{
	corotest();
}
