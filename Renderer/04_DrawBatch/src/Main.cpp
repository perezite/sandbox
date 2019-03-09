#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "DrawBatch.h"
#include "Renderer.h"
#include "Stopwatch.h"
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

void demo1()
{
	sb::Window window;
	sb::DrawBatch batch;
	sb::Stopwatch sw;

	sb::Triangle triangle1;
	sb::Triangle triangle2;
	sb::Quad quad1;
	sb::Quad quad2;

	
	triangle1.setPosition(-0.5f, 0.5f);
	triangle2.setPosition(0.5f, -0.5f);
	quad1.setPosition(0.5f, 0.5f);
	quad2.setPosition(-0.5f, -0.5f);

	while (window.isOpen()) {
		sw.reset();

		window.update();
		window.clear();

		sb::Renderer::resetStatistics();

		batch.begin(window);
		batch.draw(quad1);
			batch.draw(triangle1);
			batch.draw(triangle2);
			batch.draw(quad2);
		batch.end();

		window.display();

		static std::size_t counter = 0;
		if (counter % 200 == 0) {
			SDL_Log("Num draw calls: %d", sb::Renderer::getNumDrawCalls());
			SDL_Log("Elapsed: %f, FPS: %f", sw.getElapsedMs(), 1000 / sw.getElapsedMs());
			SDL_Log("%ld %ld", batch.getBatchedDrawCalls(), batch.getUnbatchedDrawCalls());
			SDL_Log("Batched/Unbatched: %f", (float)batch.getBatchedDrawCalls() / (float)batch.getUnbatchedDrawCalls());
		}
		counter++;
	}
}

float randomValue() {
	float rnd = float(rand()) / float(RAND_MAX);
	return rnd;
}

void init(sb::Drawable* drawable) {
	drawable->setPosition(2 * randomValue() - 1, 2 * randomValue() - 1);
	drawable->setRotation(2 * 3.1415f * randomValue());
	drawable->setScale(randomValue() * 0.2f, randomValue() * 0.2f);
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

void demo2() 
{
	sb::Window window;
	sb::Stopwatch sw;

	sb::DrawBatch batch(16384);

	std::vector<sb::Drawable*> drawables;
	init<sb::Triangle>(drawables, 2500);
	init<sb::Quad>(drawables, 2500);

	while (window.isOpen()) {
		sw.reset();

		window.update();
		window.clear();
	
		sb::Renderer::resetStatistics();
		batch.begin(window);
		for (std::size_t i = 0; i < drawables.size(); i++)
			batch.draw(drawables[i]);
		batch.end();

		window.display();

		static std::size_t counter = 0;
		counter++;
		if (counter % 60 == 0) {
			SDL_Log("Elapsed: %f, FPS: %f", sw.getElapsedMs(), 1000 / sw.getElapsedMs());
		}
	}

	cleanup(drawables);
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	demo2();

	// demo1();
}
