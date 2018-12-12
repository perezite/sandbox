#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include "DrawBatch.h"
#include <vector>
using namespace sb;

void run();
void logPerformance();

int main(int argc, char* args[])
{
	SDL_Log("Batched Renderer: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window;

	Shader halfTransparent("Shaders/HalfTransparent.vert", "Shaders/HalfTransparent.frag");
	//Shader redTint("Shaders/RedTint.vert", "Shaders/RedTint.frag");

	Triangle triangle(Vector2f(-0.5f, -0.3f), Vector2f(0.2f, 0.2f),  0.785398f);
	Triangle triangle2(Vector2f(0.5f, 0.3f), Vector2f(0.2f, 0.2f), 0.785398f, &halfTransparent);

	//DrawBatch batch;
	//batch.create<Triangle>(Vector2f(-0.5f, 0.3f), Vector2f(0.2f, 0.2f), -0.4f);
	//batch.create<Triangle>(Vector2f(0.5f, -0.3f), Vector2f(0.2f, 0.2f), -0.4f);

	//DrawBatch batch2(&redTint);
	//batch2.create<sb::Rectangle>(Vector2f(-0.1f, 0.2f), Vector2f(0.1f, 0.1f), -0.4f);
	//batch2.create<Triangle>(Vector2f( 0.2f, -0.1f), Vector2f(0.1f, 0.1f), -0.4f);

	while (window.isOpen()) {
		window.update();
		window.draw(triangle);
		//window.draw(batch);
		window.draw(triangle2);
		//window.draw(batch2);
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
