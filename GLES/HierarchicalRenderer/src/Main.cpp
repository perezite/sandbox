#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Rotator.h"
#include "Stopwatch.h"
#include "Error.h"
#include "DrawBatch.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace sb;

void run();
void update(Rotator& rotator);

void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Hierarchical Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window;

	/*sb::Rectangle rod(Vector2f(0, 0), Vector2f(1.0f, 0.03f));
	sb::Triangle leftPropeller(Vector2f(-0.5f, 0.0f), Vector2f(0.1f, 0.1f));
	sb::Triangle rightPropeller(Vector2f(0.5f, 0.0f), Vector2f(0.1f, 0.1f));*/
	Rotator rotator(Vector2f(0, 0), Vector2f(1.0f, 0.03f));

	while (window.isOpen()) {
		window.update();
		update(rotator);
		rotator.draw(window);
		window.display();
		logPerformance();
	}
}

void update(Rotator& rotator)
{
	static Stopwatch sw;
	rotator.setRotation(sw.getElapsedSeconds());
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
