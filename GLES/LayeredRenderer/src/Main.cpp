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
	rectangle.setLayer(-2);
	Triangle triangle(Vector2f(0.25f, 0.25f), Vector2f(0.5f, 0.5f));
	triangle.setLayer(-1);

	DrawBatch drawBatch1;
	drawBatch1.create<Triangle>(Vector2f(-0.1f, -0.1f), Vector2f(0.1f, 0.1f));
	drawBatch1.create<sb::Rectangle>(Vector2f(0.1f, 0.1f), Vector2f(0.1f, 0.1f), 0.5f);
	drawBatch1.setLayer(-3);

	DrawBatch drawBatch2;
	drawBatch2.create<sb::Rectangle>(Vector2f(0.2f, 0.2f), Vector2f(0.1f, 0.1f));
	drawBatch2.create<sb::Rectangle>(Vector2f(0.3f, 0.3f), Vector2f(0.2f, 0.2f), -0.3f);

	while (window.isOpen()) {
		window.update();
		window.draw(rectangle);
		window.draw(triangle);
		window.draw(drawBatch1);
		window.draw(drawBatch2);
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
