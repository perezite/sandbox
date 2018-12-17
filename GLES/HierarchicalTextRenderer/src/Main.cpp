#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "MeshList.h"
#include "Stopwatch.h"
#include "Error.h"
#include "DrawBatch.h"
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

	MeshList meshList;
	meshList.add(Triangle::TriangleMesh);
	// meshList.add(Rectangle::RectangleMesh);

	while (window.isOpen()) {
		window.update();
		window.draw(meshList);
		window.display();
	}
}

