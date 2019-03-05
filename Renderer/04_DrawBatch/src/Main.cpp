#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "DrawBatch.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <iostream>

float randomValue()
{
	float rnd = float(rand()) / float(RAND_MAX);
	return rnd;
}

void init(sb::Drawable& drawable) {
	drawable.setPosition(2 * randomValue() - 1, 2 * randomValue() - 1);
	drawable.setRotation(2 * 3.1415f * randomValue());
	drawable.setScale(randomValue() * 0.2f, randomValue() * 0.2f);
}

template <class T>
void init(std::vector<T>& triangles, std::size_t numTriangles)
{
	for (std::size_t i = 0; i < numTriangles; i++) {
		triangles.push_back(T());
		init(triangles[i]);
	}
}

void demo1() 
{
	sb::Window window;
	
	sb::DrawBatch batch;
	
	std::vector<sb::Triangle> triangles;
	init(triangles, 10);

	std::vector<sb::Quad> quads;
	init(quads, 10);

	while (window.isOpen()) {
		window.update();
		window.clear();
		
		sb::Renderer::resetStatistics();
		
		for (std::size_t i = 0; i < triangles.size(); i++)
			batch.draw(triangles[i]);
		for (std::size_t i = 0; i < quads.size(); i++)
			batch.draw(quads[i]); 
		window.draw(batch);

		std::cout << "Num draw calls: " << sb::Renderer::getNumDrawCalls() << std::endl;

		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo1();
}
