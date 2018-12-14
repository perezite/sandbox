#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include "DrawBatch.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace sb;

void run();
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Layered Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window;

	sb::Rectangle rectangle(Vector2f(0.5f, 0.5f), Vector2f(0.5f, 0.5f));
	Triangle triangle(Vector2f(0.25f, 0.25f), Vector2f(0.5f, 0.5f));

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
