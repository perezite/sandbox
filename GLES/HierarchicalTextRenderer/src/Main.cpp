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

	Triangle parent;
	parent.setScale(0.2f, 0.2f);
	parent.setPosition(0.5f, 0.5f);

	Triangle child;
	child.setScale(0.5f, 0.5f);
	child.setPosition(0.7f, -0.7f);
	child.setParent(parent);

	while (window.isOpen()) {
		window.update();
		parent.setRotation(sw.getElapsedSeconds());
		child.setRotation(-0.5f * sw.getElapsedSeconds());
		parent.draw(window);
		child.draw(window);
		window.display();
	}
}

