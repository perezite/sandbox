#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

//#define VER_1
#define VER_2

#ifdef VER_1
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
#endif

#ifdef VER_2
#define CO_CONTEXT CoContext& ctx
#define CO_DECLARE_BEGIN							\
	struct CoVariables								\
	{												
#define CO_DECLARE_END								\
	};												\
													\
	if (ctx.variables == NULL)						\
		ctx.variables = new CoVariables();			\
	CoVariables* co = (CoVariables*)ctx.variables;	
#define CO_START								\
	switch (ctx.state) {							\
		default:								\
			error("coro state not defined");	\
			break;								\
		case -1:								
#define CO_YIELD(index)							\
	do {										\
		ctx.state = index;						\
		return;									\
	case index:;								\
	} while (0)			
#define CO_AWAIT(func, index)					\
	{											\
		ctx.childContext = new CoContext();		\
		while (ctx.childContext->running) {		\
			func(*ctx.childContext);			\
			CO_YIELD(index);					\
		}										\
												\
		delete ctx.childContext;				\
	}
#define CO_FINISH								\
		delete ctx.variables;					\
		ctx.running = false;					\
		return;									\
	}											\
	delete ctx.variables;						\
	ctx.running = false;						\
	return;														
#endif

void error(const char* message)
{
	SDL_Log("%s", message);
	std::cin.get();
	exit(0);
}

struct CoContext
{
	void* variables = NULL;
	CoContext* childContext;
	int state = -1;
	bool running = true;
};

void coro5_child(CO_CONTEXT)
{
	CO_START;
	
	SDL_Log("Coro Child: 1");
	CO_YIELD(0);

	SDL_Log("Coro Child: 2");
	CO_YIELD(1);

	CO_FINISH;
}

void coro5(CO_CONTEXT)
{
	CO_START;

	CO_AWAIT(coro5_child, 0);

	SDL_Log("Once again...");

	CO_AWAIT(coro5_child, 1);

	CO_FINISH;
}

void run_coro5()
{
	CoContext ctx;
	while (ctx.running) {
		SDL_Log("Main: %d", ctx.running);
		coro5(ctx);
	}
}

void coro4(CO_CONTEXT)
{
	CO_DECLARE_BEGIN;
		int foo = 42;
	CO_DECLARE_END;

	CO_START;

	SDL_Log("Coro4: %d", co->foo);
	co->foo = 43;
	CO_YIELD(0);

	SDL_Log("Coro4: %d", co->foo);
	CO_FINISH;
}

void run_coro4()
{
	CoContext ctx;
	while (ctx.running) {
		SDL_Log("Main: %d", ctx.running);
		coro4(ctx);
	}
}

/*
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
*/

int main(int argc, char* args[])
{
	run_coro5();

	std::cin.get();

	return 0;
}
