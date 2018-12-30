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
		ctx.running = true;							\
		ctx.state = index;							\
		return ctx;									\
	case index:;									\
	} while (0)			
#define CO_AWAIT(func, index)						\
	{												\
		ctx.childContext = new CoContext();			\
		do {										\
			func(*ctx.childContext);				\
			CO_YIELD(index);						\
		} while(ctx.childContext->running);			\
													\
		delete ctx.childContext;					\
	}
#define CO_STEP(func, context, index)				\
	func(context); CO_YIELD(index);
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

CoContext& coro_c(CO_CONTEXT) 
{
	CO_DECLARE_BEGIN;
	int i = 0;
	CO_DECLARE_END;

	CO_BEGIN;
	for (co->i = 0; co->i < 3; co->i++) {
		SDL_Log("Hello C %d", co->i);
		CO_YIELD(0);
	}

	CO_END;
}

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
	CoContext c;
	int i;
	CO_DECLARE_END;

	// Execute coro_b step-by-step
	CO_BEGIN;
	SDL_Log("Execute coro_b step-by-step...");
	SDL_Log("Hello A 1");
	CO_STEP(coro_b, co->b, 1);
	SDL_Log("Hello A 2");
	CO_STEP(coro_b, co->b, 2);

	// Execute coro_b step-by-step in a loop
	SDL_Log("Execute coro_b step-by-step in a loop...");
	while (co->b2.running) {
		SDL_Log("Hello A 3");
		CO_STEP(coro_b, co->b2, 3);
	}

	// Await coro_b
	SDL_Log("Await coro_b...");
	CO_AWAIT(coro_b, 4);

	// Execute coro_c step-by-step in a loop
	SDL_Log("Execute coro_c step-by-step in a loop");
	while (co->c.running) {
		SDL_Log("Hello A 4");
		CO_STEP(coro_c, co->c, 5);
	}

	// Await coro_c twice
	SDL_Log("Await coro_c twice");
	for (co->i = 0; co->i < 2; co->i++) {
		SDL_Log("Hello A 5");
		CO_AWAIT(coro_c, 6);
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

int main(int argc, char* args[])
{
	run_coro();

	std::cin.get();

	return 0;
}
