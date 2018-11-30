#include <iostream>
#include <vector>
#include "Window.h"
#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
using namespace sb;

const unsigned int NumTrianglesHorz = 2; 
const unsigned int NumTrianglesVert = 2; 

void run();
void spawnDrawables(std::vector<Drawable*>& drawables);
void showDrawables(std::vector<Drawable*>& drawables, Window& window);
void logPerformance();
void update(std::vector<Drawable*>& drawables, Window& window);

void run2();
void spawnDrawables2(std::vector<Drawable*>& drawables);
void showDrawables2(std::vector<Drawable*>& drawables, Window& window);
void update2(std::vector<Drawable*>& drawables, Window& window);

int main(int argc, char* args[])
{
	SDL_Log("Indexed Renderer: Build %s %s", __DATE__, __TIME__);

	run2();
}

void run2()
{
	std::vector<Drawable*> drawables;

	Window window;
	spawnDrawables2(drawables);
	showDrawables2(drawables, window);

	while (window.isOpen()) {
		window.update();
		window.draw();
		update2(drawables, window);
	}
}

void spawnDrawables2(std::vector<Drawable*>& drawables)
{
	drawables.push_back(new Line(Vector2f(0, 0)));
	drawables.push_back(new sb::Rectangle(Vector2f(0, 0)));
	drawables.push_back(new Triangle(Vector2f(0, 0)));
}

void showDrawables2(std::vector<Drawable*>& drawables, Window& window)
{
	for (std::size_t i = 0; i < drawables.size(); i++)
		window.show(drawables[i]);
}

void update2(std::vector<Drawable*>& drawables, Window& window) {
	std::size_t numToRemove = rand() % 2;
	std::cout << "Deleted: ";
	for (std::size_t i = 0; i < numToRemove; i++) {
		int rnd = rand() % drawables.size();
		window.hide(drawables[rnd]); drawables.erase(drawables.begin() + rnd);
	}

	std::size_t numToAdd = rand() % 2;
	std::cout << "Added: ";
	for (std::size_t i = 0; i < numToAdd; i++) {
		int rnd = rand() % 4;
		Drawable* drw = rnd == 0 ? (Drawable*)new Point() : rnd == 1 ? (Drawable*)new Line() : rnd == 2 ? (Drawable*)new Triangle() : (Drawable*)new sb::Rectangle();
		drawables.push_back(drw); window.show(drw); 
		std::cout << (rnd == 0 ? "Point" : rnd == 1 ? "Line" : rnd == 2 ? "Triangle" : "Rectangle");
	}
}

/*************************************************************************************/

void run()
{
	std::vector<Drawable*> drawables;

	Window window;
	spawnDrawables(drawables);
	showDrawables(drawables, window);

	while (window.isOpen()) {
		window.update();
		// update(drawables, window);
		window.draw();
		logPerformance();
	}
}

void spawnDrawables(std::vector<Drawable*>& drawables)
{
	float stepWidth = 2.0f / NumTrianglesHorz;
	float stepHeight = 2.0f / NumTrianglesVert;
	Vector2f scale(0.9f * stepWidth, 0.9f * stepHeight);

	int count = 0;
	for (std::size_t i = 0; i < NumTrianglesHorz; i++) {
		for (std::size_t j = 0; j < NumTrianglesVert; j++) {
			Vector2f position(-1.0f + (i + 0.5f) * stepWidth, -1.0f + (j + 0.5f) * stepHeight);
			drawables.push_back(count % 2 == 0 ?
				(Drawable*)new sb::Rectangle(position, scale, 10 * ToRadian) :
				(Drawable*)new sb::Triangle(position, scale, 10 * ToRadian));
			count++;
		}
		count++;
	}
}

void showDrawables(std::vector<Drawable*>& drawables, Window& window)
{
	for (std::size_t i = 0; i < drawables.size(); i++)
		window.show(drawables[i]);
}

void update(std::vector<Drawable*>& drawables, Window& window)
{
	static Stopwatch sw;
	static int count = 0;
	static int index = -1;

	if (sw.getElapsedSeconds() > 0.1) {
		if (count % 2 == 0) {
			index = rand() % drawables.size();
			window.hide(drawables[index]);
		}
		else
			window.show(drawables[index]);
		
		sw.reset();
		count++;
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
