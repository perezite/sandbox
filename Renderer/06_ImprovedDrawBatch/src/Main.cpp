#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "DrawBatch.h"
#include "Renderer.h"
#include "Stopwatch.h"
#include "Math.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>

/*void init(sb::Drawable* drawable) {
	drawable->setPosition(sb::random(-1, 1), sb::random(-1, 1));
	drawable->setRotation(sb::random(2 * sb::Pi));
	drawable->setScale(sb::random(0.2f), sb::random(0.2f));
}

template <class T>
void init(std::vector<sb::Drawable*>& shapes, std::size_t numShapes) {
	for (std::size_t i = 0; i < numShapes; i++) {
		sb::Drawable* drawable = (sb::Drawable*)new T();
		init(drawable);
		shapes.push_back(drawable);
	}
}

void cleanup(std::vector<sb::Drawable*> drawables) {
	for (std::size_t i = 0; i < drawables.size(); i++)
		delete drawables[i];

	drawables.clear();
}

void printStatistics(sb::Stopwatch& sw) {
	static std::size_t counter = 0;
	counter++;
	if (counter % 60 == 0) 
		SDL_Log("Elapsed: %f, FPS: %f", sw.getElapsedMs(), 1000 / sw.getElapsedMs());
}

void demo2() 
{
	sb::Window window;
	// sb::DrawBatch batch;
	sb::Stopwatch sw;

	std::vector<sb::Drawable*> drawables;
	init<sb::Triangle>(drawables, 2);
	init<sb::Quad>(drawables, 2);

	while (window.isOpen()) {
		sw.reset();

		window.update();
		window.clear();
	
		// batch.begin(window);
		for (std::size_t i = 0; i < drawables.size(); i++)
			batch.draw(drawables[i]);
		// batch.end();


		//for (std::size_t i = 0; i < drawables.size(); i++)
		//	batch.draw(drawables[i]);
		//window.draw(batch);

		window.display();

		printStatistics(sw);
	}

	cleanup(drawables);
}
*/

void init0(std::vector<sb::Triangle>& triangles, std::vector<sb::Quad>& quads) 
{
	triangles[0].setPosition(-0.5f, 0.5f);
	triangles[1].setPosition(0.5f, -0.5f);
	quads[0].setPosition(0.5f, 0.5f);
	quads[1].setPosition(-0.5f, -0.5f);
}

void printStats0() {
	static std::size_t counter = 0;
	
	if (counter % 100 == 0)
		SDL_Log("%d", sb::Renderer::getNumDrawCalls());

	sb::Renderer::resetStatistics();
	counter++;
}

void demo0()
{
	sb::Window window;
	sb::DrawBatch batch;
	sb::Stopwatch sw;

	std::vector<sb::Triangle> triangles(2);
	std::vector<sb::Quad> quads(2);
	init0(triangles, quads);

	while (window.isOpen()) {
		window.update();
		
		window.clear();
		batch.draw(triangles[0]);
		batch.draw(triangles[1]);
		batch.draw(quads[0]);
		batch.draw(quads[1]);
		window.draw(batch);

		window.display();

		printStats0();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("DrawBatch Renderer: Build %s %s", __DATE__, __TIME__);

	demo0();

	// demo1();
}
