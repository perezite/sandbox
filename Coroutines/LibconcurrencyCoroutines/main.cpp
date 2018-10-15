
extern "C"
{
	#include "coro.h"
}
#include <stdio.h>

coro cr;
coro cw;

void reader(cvalue r) {
	for (unsigned int i = 0; i < 5; i++) 
	{
		printf("R\n");
		coro_call(cw, cnone);
	}
}

void writer(cvalue w) {
	for (unsigned int i = 0; i < 5; i++) 
	{
		printf("W\n");
		coro_call(cr, cnone);
	}
}

int main(int argc, char ** argv) {
	coro _main = coro_init();
	printf("Simple reader/writer echo...\n");
	cr = coro_new(reader);
	cw = coro_new(writer);
	coro_call(cw, cnone);
	return 0;
}

