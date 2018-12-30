#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

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
	switch (ctx.state) {						\
		default:								\
			error("coro state not defined");	\
			break;								\
		case -1:								
#define CO_YIELD(index)							\
	do {										\
		ctx.running = true;						\
		ctx.state = index;						\
		return;									\
	case index:;								\
	} while (0)			
#define CO_AWAIT(func, index)					\
	{											\
		ctx.childContext = new CoContext();		\
		do {									\
			func(*ctx.childContext);			\
			CO_YIELD(index);					\
		} while(ctx.childContext->running);		\
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
	bool running = false;
};

void coro6_child(CO_CONTEXT)
{
	CO_DECLARE_BEGIN;
		int i = 0;
	CO_DECLARE_END;

	CO_START;
	while (co->i < 3) {
		SDL_Log("Coro6 Child: i=%d", co->i);
		co->i++;
		CO_YIELD(0);
	}
	CO_FINISH;
}

void coro6(CO_CONTEXT)
{
	CO_DECLARE_BEGIN;
	int i = 42;
	CO_DECLARE_END;

	CO_START;
	for (co->i = 0; co->i < 3; co->i++) {
		SDL_Log("Coro6: i=%d", co->i);
		CO_AWAIT(coro6_child, 0);
	}

	CO_FINISH;
}

void run_coro6()
{
	CoContext ctx;
	do {
		SDL_Log("Main: %d", ctx.running);
		coro6(ctx);
	} while (ctx.running);
}

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
	do {
		SDL_Log("Main: %d", ctx.running);
		coro5(ctx);
	} while (ctx.running);
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
	do {
		SDL_Log("Main: %d", ctx.running);
		coro4(ctx);
	} while (ctx.running);
}

int main(int argc, char* args[])
{
	run_coro6();

	std::cin.get();

	return 0;
}
