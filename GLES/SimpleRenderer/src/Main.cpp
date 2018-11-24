#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <SDL2/SDL.h>

void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: %s %s", __DATE__, __TIME__);

	sb::Window window;

	sb::Triangle triangle;
	window.show(triangle);

	while (window.isOpen())
	{
		window.update();
		window.display();
		logPerformance();
	}
}

void logPerformance()
{
	static sb::Stopwatch stopwatch;
	static unsigned int frames = 0;

	float elapsed = stopwatch.getElapsedSeconds();
	frames++;
	if (elapsed > 1.0f) {
		float fps = frames / elapsed;
		SDL_Log("FPS: %f", fps);
		frames = 0;
		stopwatch.reset();
	}
}
