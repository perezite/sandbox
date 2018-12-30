#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
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

struct CoContext
{
	void* variables = NULL;
	CoContext* childContext;
	int state = -1;
	bool running = true;
};

struct Coroutine {
	CoContext context;
	CoContext& (*routine)(CO_CONTEXT);
};

static std::vector<Coroutine> Coroutines;
static std::vector<Coroutine> ScheduledCoroutines;

void startCoroutine(CoContext& (*routine)(CO_CONTEXT))
{
	Coroutine co;
	co.routine = routine;
	ScheduledCoroutines.push_back(co);
}

bool coroutineFinished(Coroutine& co)
{
	return !co.context.running;
}

void runCoroutines()
{
	Coroutines.insert(Coroutines.end(), ScheduledCoroutines.begin(), ScheduledCoroutines.end());
	ScheduledCoroutines.clear();

	std::vector<Coroutine>::iterator iter = std::remove_if(Coroutines.begin(), Coroutines.end(), coroutineFinished);
	if (iter != Coroutines.end())
		Coroutines.erase(iter, Coroutines.end());

	SDL_Log("Hello Scheduler");
	for (std::size_t i = 0; i < Coroutines.size(); i++) 
		Coroutines[i].routine(Coroutines[i].context);
}

CoContext& coro_d(CO_CONTEXT)
{
	CO_BEGIN;

	SDL_Log("Hello D 1");
	CO_YIELD(0);
	SDL_Log("Hello D 2");

	CO_END;
}

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

	CO_BEGIN;
	SDL_Log("Execute coro_b step-by-step...");
	SDL_Log("Hello A 1");
	CO_STEP(coro_b, co->b, 1);
	SDL_Log("Hello A 2");
	CO_STEP(coro_b, co->b, 2);

	SDL_Log("Execute coro_b step-by-step in a loop...");
	while (co->b2.running) {
		SDL_Log("Hello A 3");
		CO_STEP(coro_b, co->b2, 3);
	}

	SDL_Log("Await coro_b...");
	CO_AWAIT(coro_b, 4);

	SDL_Log("Execute coro_c step-by-step in a loop");
	while (co->c.running) {
		SDL_Log("Hello A 4");
		CO_STEP(coro_c, co->c, 5);
	}

	SDL_Log("Await coro_c twice");
	for (co->i = 0; co->i < 2; co->i++) {
		SDL_Log("Hello A 5");
		CO_AWAIT(coro_c, 6);
	}

	CO_END;
}

bool activeCoroutines()
{
	return !Coroutines.empty() || !ScheduledCoroutines.empty();
}

void run_coro_a_and_d()
{
	startCoroutine(&coro_d);
	startCoroutine(&coro_a);
	while (activeCoroutines())
		runCoroutines();
}

CoContext& coro_f(CO_CONTEXT)
{
	CO_BEGIN;
	SDL_Log("F 1");
	CO_YIELD(0);
	SDL_Log("F 2");
	CO_END;
}

CoContext& coro_e(CO_CONTEXT) 
{
	CO_BEGIN;
	SDL_Log("E 1");
	startCoroutine(&coro_f);
	CO_YIELD(0);
	SDL_Log("E 2");
	CO_YIELD(1);
	SDL_Log("E 3");
	CO_END;
}

void run_coro_e()
{
	startCoroutine(&coro_e);

	while (activeCoroutines())
		runCoroutines();
}

int main(int argc, char* args[])
{
	// run_coro_a_and_d();
	run_coro_e();

	std::cin.get();

	return 0;
}
