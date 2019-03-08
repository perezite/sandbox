#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "DrawBatch.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

float randomValue() {
	float rnd = float(rand()) / float(RAND_MAX);
	return rnd;
}

void init(sb::Shape* shape) {
	shape->setPosition(2 * randomValue() - 1, 2 * randomValue() - 1);
	shape->setRotation(2 * 3.1415f * randomValue());
	shape->setScale(randomValue() * 0.2f, randomValue() * 0.2f);
}

void init(std::vector<sb::Shape*>& shapes, std::size_t numShapes) {
	for (std::size_t i = 0; i < numShapes; i++) {
			sb::Shape* shape = i % 2 == 0 ? 
			(sb::Shape*)new sb::Triangle() : (sb::Shape*)new sb::Quad();
		init(shape);
		shapes.push_back(shape);
	}
}

void cleanup(std::vector<sb::Shape*> shapes) {
	for (std::size_t i = 0; i < shapes.size(); i++)
		delete shapes[i];

	shapes.clear();
}

void demo1() 
{
	srand(42);

	sb::Window window;
	
	sb::DrawBatch batch(16384);

	std::vector<sb::Shape*> shapes;
	init(shapes, 5000);

	while (window.isOpen()) {
		window.update();
		window.clear();
		
		sb::Renderer::resetStatistics();
		
		for (std::size_t i = 0; i < shapes.size(); i++)
			batch.draw(shapes[i]);

		window.draw(batch);

		SDL_Log("Num draw calls: %d", sb::Renderer::getNumDrawCalls());

		window.display();
	}

	cleanup(shapes);
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo1();
}
