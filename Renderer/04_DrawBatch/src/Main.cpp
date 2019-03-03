#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>

float randomValue()
{
	float rnd = float(rand()) / float(RAND_MAX);
	return rnd;
}

void init(sb::Drawable& drawable) {
	drawable.setPosition(2 * randomValue() - 1, 2 * randomValue() - 1);
	drawable.setRotation(2 * 3.1415f * randomValue());
	drawable.setScale(randomValue(), randomValue());
}

void init(std::vector<sb::Triangle>& triangles, std::size_t numTriangles)
{
	for (std::size_t i = 0; i < numTriangles; i++) {
		triangles.push_back(sb::Triangle());
		init(triangles[i]);
	}
}

void demo1() 
{
	sb::Window window;
	
	// sb::DrawBatch batch;
	
	std::vector<sb::Triangle> triangles;
	init(triangles, 50);

	while (window.isOpen()) {
		window.update();
		window.clear();
		
		// for (std::size_t i = 0; i < triangles.size(); i++)
		//	batch.draw(triangle[i]);	
		// window.draw(batch);

		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo1();
}
