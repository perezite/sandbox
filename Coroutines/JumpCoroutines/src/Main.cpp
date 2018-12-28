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



