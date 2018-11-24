#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <SDL2/SDL.h>
#include <vector>
using namespace sb;

void run();
void spawnTriangles(std::vector<Drawable*>& triangles);
void showTriangles(std::vector<Drawable*>& triangles, Window& window);
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	std::vector<Drawable*> triangles;

	Window window;
	spawnTriangles(triangles);
	showTriangles(triangles, window);

	while (window.isOpen()) {
		window.update();
		window.display();
		logPerformance();
	}
}

void spawnTriangles(std::vector<Drawable*>& triangles)
{
	unsigned int numTrianglesHorz = 10;
	unsigned int numTrianglesVert = 10;
	float stepWidth = 2.0f / numTrianglesHorz;
	float stepHeight = 2.0f / numTrianglesVert;
	Vector2f scale(0.9f * stepWidth, 0.9f * stepHeight);

	for (std::size_t i = 0; i < numTrianglesHorz; i++) {
		for (std::size_t j = 0; j < numTrianglesVert; j++) {
			Vector2f position(-1.0f + (i + 0.5f) * stepWidth, -1.0f + (j + 0.5f) * stepHeight);
			triangles.push_back(new Triangle(position, scale));
		}
	}
}

void showTriangles(std::vector<Drawable*>& triangles, Window& window)
{
	for (std::size_t i = 0; i < triangles.size(); i++)
		window.show(triangles[i]);
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

