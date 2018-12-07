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

const unsigned int NumTrianglesHorz = 1; 
const unsigned int NumTrianglesVert = 1; 

void run();
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Indexed Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window(800, 800);

	Triangle triangle(Vector2f(-0.5f, 0.5f), Vector2f (0.3f, 0.3f));
	sb::Rectangle rectangle(Vector2f(-0.2f, 0.1f), Vector2f(0.2f, 0.2f), 30 * sb::ToRadian);
	window.show(&triangle);
	window.show(&rectangle);

	/* 
	DrawBatch batch;
	for (std::size_t i = 0; i < 100; i++) {
		Vector2f pos(rand() % 100 / 100.0f * 0.5f, rand() % 100 / 100.0f * 0.5f);
		batch.add(Triangle(pos, Vector2f(0.1f, 0.1f));		// https://stackoverflow.com/questions/84427/is-it-legal-to-pass-a-newly-constructed-object-by-reference-to-a-function
	}
	
	window.show(batch);
	*/


	while (window.isOpen()) {
		window.update();
		window.draw();
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
