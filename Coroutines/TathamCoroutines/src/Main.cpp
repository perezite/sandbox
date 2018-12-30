#include <stdio.h>
#include <iostream>
#include <map>
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
#define CO_BEGIN									\
	ctx.running = false;							\
	switch (ctx.state) {							\
		default:									\
			error("coro state not defined");		\
			break;									\
		case -1:									
#define CO_YIELD(index)								\
	do {											\
		ctx.running = true;						\
		ctx.state = index;							\
		return ctx;									\
	case index:;									\
	} while (0)			
#define CO_AWAIT(func, index)						\
	{												\
		ctx.childContext = new CoContext();		\
		do {										\
			func(*ctx.childContext);				\
			CO_YIELD(index);						\
		} while(ctx.childContext->running);		\
													\
		delete ctx.childContext;					\
	}
#define CO_END										\
		delete ctx.variables;						\
		ctx.running = false;						\
		return ctx;									\
	}												\
	delete ctx.variables;							\
	ctx.running = false;							\
	return ctx;									

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

CoContext& coro_b(CO_CONTEXT)
{
	CO_BEGIN; 
	
	SDL_Log("Hello B 1");
	CO_YIELD(0);
	SDL_Log("Hello B 2");

	CO_END;
}

CoContext& coro_a(CO_CONTEXT)
{
	CO_DECLARE_BEGIN;
	CoContext b;
	CoContext b2;
	CO_DECLARE_END;

	CO_BEGIN;
	SDL_Log("Hello A 1");
	coro_b(co->b);
	CO_YIELD(1);

	SDL_Log("Hello A 2");
	coro_b(co->b);
	CO_YIELD(2);

	while (coro_b(co->b2).running) {
		SDL_Log("Hello A3");
		CO_YIELD(3);
	}
	CO_END;
}

void run_coro()
{
	CoContext ctx;
	while (coro_a(ctx).running) {
		SDL_Log("Main: %d", ctx.running);
	}
}

/*
void coro_child(CO_CONTEXT)
{
	CO_DECLARE_BEGIN;
		int i = 0;
	CO_DECLARE_END;

	CO_BEGIN;
	while (co->i < 3) {
		SDL_Log("Coro6 Child: i=%d", co->i);
		co->i++;
		CO_YIELD(0);
	}
	CO_END;
}

void coro(CO_CONTEXT)
{
	CO_DECLARE_BEGIN;
	int i = 42;
	CO_DECLARE_END;

	CO_BEGIN;
	for (co->i = 0; co->i < 3; co->i++) {
		SDL_Log("Coro6: i=%d", co->i);
		CO_AWAIT(coro_child, 0);
	}

	CO_END;
}

void run_coro()
{
	CoContext ctx;
	do {
		SDL_Log("Main: %d", ctx.running);
		coro(ctx);
	} while (ctx.running);
}*/

int main(int argc, char* args[])
{
	run_coro();

	std::cin.get();

	return 0;
}
