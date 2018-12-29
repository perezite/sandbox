#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

#define CO_START								\
	static int state = -1;						\
												\
	switch (state) {							\
		default:								\
			error("coro state not defined");	\
			break;								\
		case -1:								
#define CO_YIELD(index)							\
	do {										\
		state = index;							\
		return 0;								\
	case index:;								\
	} while (0)			
#define CO_FINISH								\
		return -1;								\
	}											\
	return -1;					
#define CO_AWAIT(i, func)						\
while (true) {									\
	if (func() < 0)								\
		break;									\
	CO_YIELD(i);								\
}

void error(const char* message)
{
	SDL_Log("%s", message);
	std::cin.get();
	exit(0);
}

int coro(void) 
{
	static int i;
	CO_START;

	for (i = 0; i < 10; i++) {
		SDL_Log("%d", i);
		CO_YIELD(0);
	}

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

int coro2()
{
	CO_START;

	SDL_Log("Hello 1");
	CO_YIELD(0);

	SDL_Log("Hello 2");
	CO_FINISH;
}

void run_coro2()
{
	int val = 0;
	while (val > -1) {
		SDL_Log("Main: %d", val);
		val = coro2();
	}
}

int coro3_child()
{
	CO_START;

	SDL_Log("Hello Child 1");
	CO_YIELD(0);

	SDL_Log("Hello Child 2");
	CO_YIELD(1);

	CO_FINISH;
}

int coro3()
{
	CO_START;

	SDL_Log("Hello Parent 1");
	CO_YIELD(0);

	CO_AWAIT(1, coro3_child);

	SDL_Log("Hello Parent 2");

	CO_FINISH;
}

void run_coro3()
{
	int val = 0;
	while (val > -1) {
		SDL_Log("Main: %d", val);
		val = coro3();
	}
}

struct CoContext
{
	void* variables = NULL;

	bool running = true;
};

struct CoVariables
{
	int foo = 42;
};

void coro4(CoContext& ctx)
{
	if (ctx.variables == NULL)
		ctx.variables = new CoVariables();
	CoVariables* var = (CoVariables*)ctx.variables;
	static int state = -1;					
	switch (state) {
	case -1:
		SDL_Log("Hi there");
		var->foo = 43;
		state = 0;
		return;
	case 0:
		SDL_Log("%d", var->foo);

		delete ctx.variables;
		ctx.running = false;
		return;
	}

	delete ctx.variables;
	ctx.running = false;
	return;
}

void run_coro4()
{
	CoContext ctx;
	while (ctx.running) {
		SDL_Log("Main: %d", ctx.running);
		int test = 0;
		if (ctx.variables != NULL)
			test = ((CoVariables*)ctx.variables)->foo;
		coro4(ctx);
		int test2 = ((CoVariables*)ctx.variables)->foo;
		int dummy = 2;
	}
}

int main(int argc, char* args[])
{
	run_coro4();

	std::cin.get();

	return 0;
}
