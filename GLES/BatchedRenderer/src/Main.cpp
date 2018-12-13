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
void run2();
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Batched Renderer: Build %s %s", __DATE__, __TIME__);

	// run();
	run2();
}

void run()
{
	Window window;

	Shader halfTransparent("Shaders/HalfTransparent.vert", "Shaders/HalfTransparent.frag");
	Shader redTint("Shaders/RedTint.vert", "Shaders/RedTint.frag");

	Triangle triangle(Vector2f(-0.5f, -0.3f), Vector2f(0.2f, 0.2f), 0.785398f);
	Triangle triangle2(Vector2f(0.5f, 0.3f), Vector2f(0.2f, 0.2f), 0.785398f, &redTint);

	DrawBatch batch(&halfTransparent);
	batch.create<Triangle>(Vector2f(-0.5f, 0.3f), Vector2f(0.2f, 0.2f), -0.4f);
	batch.create<Triangle>(Vector2f(0.5f, -0.3f), Vector2f(0.2f, 0.2f), 0.25f);

	DrawBatch batch2;
	batch2.create<sb::Rectangle>(Vector2f(-0.1f, 0.2f), Vector2f(0.1f, 0.1f), -0.4f);
	batch2.create<Triangle>(Vector2f(0.2f, -0.1f), Vector2f(0.1f, 0.1f), -0.4f);

	while (window.isOpen()) {
		window.update();
		window.draw(triangle);
		window.draw(batch);
		window.draw(triangle2);
		window.draw(batch2);
		window.display();
		logPerformance();
	}
}

void run2()
{
	const std::size_t numHorz = 86;
	const std::size_t numVert = 86;
	float stepHorz = 2.0f / numHorz;
	float stepVert = 2.0f / numVert;

	Window window;

	DrawBatch batch;
	for (std::size_t i = 0; i < numHorz; i++) {
		for (std::size_t j = 0; j < numVert; j++) {
			Vector2f position = Vector2f(-1 + (i + 0.5f) * stepHorz, -1 + (j + 0.5f) * stepVert);
			Vector2f scale = Vector2f(0.8f * stepHorz, 0.8f * stepVert);
			float alpha = (float(rand() % RAND_MAX) / float(RAND_MAX)) * 6.28318530718f;
			if ((i % 2 + j) % 2 == 0) 
				batch.create<Triangle>(position, scale, alpha);
			else 
				batch.create<sb::Rectangle>(position, scale, alpha);
		}
	}

	while (window.isOpen()) {
		window.update();
		window.draw(batch);
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
