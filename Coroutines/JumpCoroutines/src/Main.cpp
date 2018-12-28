/*#include <setjmp.h>
#include <SDL2/SDL.h>

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
		SDL_Log("Coro");
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

	SDL_Log("Coro2 1");

	if (setjmp(Coro2) == 0) {
		longjmp(Main, 1);
	} else
		ctx = co2Context.getVars<Variables>();

	SDL_Log("Coro2 2");
	if (setjmp(Coro2) == 0) {
		longjmp(Main, 1);
	} else
		ctx = co2Context.getVars<Variables>();

	SDL_Log("Coro2 3");
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
		SDL_Log("Main");
		if (setjmp(Main) == 0)
			longjmp(Coro, 1);
	}

	// coro2
	if (setjmp(Main) == 0)
		coro2();

	while (Coro2Yielded)
	{
		SDL_Log("Main");
		if (setjmp(Main) == 0)
			longjmp(Coro2, 1);
	}
}*/

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

#define CO_BEGIN								\
	static int state = -1;						\
												\
	switch (state) {							\
		default:								\
			error("coro state not defined");	\
			break;								\
		case -1:								
#define CO_YIELD(i)								\
	do {										\
		state = i;								\
		return 0;								\
	case i:;									\
	} while (0)			
#define CO_FINISH								\
		return -1;								\
	}											\
	return -1;					

void error(const char* message)
{
	SDL_Log(message);
	std::cin.get();
	exit(0);
}

int coro(void) 
{
	static int i;
	CO_BEGIN;

	for (i = 0; i < 10; i++) {
		SDL_Log("%d", i);
		CO_YIELD(0);
	}

	CO_FINISH;
}

int coro2()
{
	CO_BEGIN;

	SDL_Log("Hello 1");
	CO_YIELD(0);

	SDL_Log("Hello 2");
	CO_FINISH;
}

void run_coro()
{
	int val = 0;
	while (val > -1) {
		val = coro();
		SDL_Log("Main: %d", val);
	}
}

void run_coro2()
{
	int val = 0;
	while (val > -1) {
		val = coro2();
		SDL_Log("Main: %d", val);
	}
}

int main(int argc, char* args[])
{
	run_coro2();

	std::cin.get();

	return 0;
}



