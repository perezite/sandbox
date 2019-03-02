#include "Window.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <iostream>

void demo1() 
{
	sb::Window window;

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo1();
}
