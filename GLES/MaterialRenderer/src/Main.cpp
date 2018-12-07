#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
using namespace sb;

void run();
void cleanup(DrawBatch& batch);
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Indexed Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window(800, 800);
	 
	DrawBatch batch1;
	for (std::size_t i = 0; i < 100; i++) {
		Vector2f pos(2 * (rand() % 100 / 100.0f) - 1, 2 * (rand() % 100 / 100.0f) - 1);
		float alpha = rand() % 100 / 100.0f * 6.28318530718f;
		batch1.add(new Triangle(pos, Vector2f(0.035f, 0.035f), alpha));
	}

	DrawBatch batch2;
	for (std::size_t i = 0; i < 100; i++) {
		Vector2f pos(2 * (rand() % 100 / 100.0f) - 1, 2 * (rand() % 100 / 100.0f) - 1);
		float alpha = rand() % 100 / 100.0f * 6.28318530718f;
		batch2.add(rand() % 2 == 0 ? (Drawable*)new Triangle(pos, Vector2f(0.02f, 0.02f), alpha) : (Drawable*)new sb::Rectangle(pos, Vector2f(0.02f, 0.02f), alpha));
	}

	Triangle triangle(Vector2f(-0.5f, 0.5f), Vector2f(0.3f, 0.3f));
	sb::Rectangle rectangle(Vector2f(-0.2f, 0.1f), Vector2f(0.2f, 0.2f), 30 * sb::ToRadian);

	window.show(&batch1);
	window.show(&batch2);
	window.show(&triangle);
	window.show(&rectangle);

	while (window.isOpen()) {
		window.update();
		window.draw();
		logPerformance();
	}

	cleanup(batch1);
	cleanup(batch2);
}

void cleanup(DrawBatch& batch)
{
	for (std::size_t i = 0; i < batch.getDrawableCount(); i++)
		delete batch[i];
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
