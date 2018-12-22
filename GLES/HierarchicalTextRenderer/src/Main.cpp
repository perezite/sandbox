#include "Window.h"
#include "Triangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace sb;

void run();
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Hierarchical Renderer (Text): Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window;
	Stopwatch sw;
	Triangle triangle;

	triangle.setScale(0.1f, 0.1f);
	triangle.setPosition(0.5f, 0.5f);

	while (window.isOpen()) {
		window.update();
		triangle.setRotation(sw.getElapsedSeconds());
		triangle.draw(window);
		window.display();
	}
}

