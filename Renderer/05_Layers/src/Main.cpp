#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "DrawBatch.h"
#include "Renderer.h"
#include "Stopwatch.h"
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

void demo2() 
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

	demo2();
}
