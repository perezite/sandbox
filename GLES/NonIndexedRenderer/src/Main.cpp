#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <SDL2/SDL.h>
#include <vector>
using namespace sb;

const unsigned int NumTrianglesHorz = 10; 
const unsigned int NumTrianglesVert = 10; 

void run();
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window;
	Triangle triangle(Vector2f(-0.5f, -0.3f), Vector2f(0.2f, 0.2f), 0.785398f);
	sb::Rectangle rectangle(Vector2f(0.5f, 0.3f), Vector2f(0.1f, 0.1f), 0);

	while (window.isOpen()) {
		window.update();
		window.draw(triangle);
		window.draw(rectangle);
		window.display();
		logPerformance();
	}
}

void logPerformance()
{
	static Stopwatch stopwatch;
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

