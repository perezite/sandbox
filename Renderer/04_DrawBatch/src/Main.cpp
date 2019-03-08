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

//float randomValue() {
//	float rnd = float(rand()) / float(RAND_MAX);
//	return rnd;
//}
//
//void init(sb::Shape* shape) {
//	shape->setPosition(2 * randomValue() - 1, 2 * randomValue() - 1);
//	shape->setRotation(2 * 3.1415f * randomValue());
//	shape->setScale(randomValue() * 0.2f, randomValue() * 0.2f);
//}
//
//void init(std::vector<sb::Shape*>& shapes, std::size_t numShapes) {
//	for (std::size_t i = 0; i < numShapes; i++) {
//			sb::Shape* shape = i % 2 == 0 ? 
//			(sb::Shape*)new sb::Triangle() : (sb::Shape*)new sb::Quad();
//		init(shape);
//		shapes.push_back(shape);
//	}
//}
//
//void cleanup(std::vector<sb::Shape*> shapes) {
//	for (std::size_t i = 0; i < shapes.size(); i++)
//		delete shapes[i];
//
//	shapes.clear();
//}
//
//void demo2() 
//{
//	sb::Window window;
//	sb::Stopwatch sw;
//
//	sb::DrawBatch batch(16384);
//
//	std::vector<sb::Shape*> shapes;
//	init(shapes, 5000);
//
//	while (window.isOpen()) {
//		sw.reset();
//
//		window.update();
//		window.clear();
//	
//		sb::Renderer::resetStatistics();		
//		for (std::size_t i = 0; i < shapes.size(); i++)
//			batch.draw(shapes[i]);
//
//		window.draw(batch);
//		window.display();
//
//		static std::size_t counter = 0;
//		counter++;
//		if (counter % 60 == 0) {
//			SDL_Log("Num draw calls: %d", sb::Renderer::getNumDrawCalls());
//			SDL_Log("Elapsed: %f, FPS: %f", sw.getElapsedMs(), 1000 / sw.getElapsedMs());
//		}
//	}
//
//	cleanup(shapes);
//}

void demo1()
{
	sb::Window window;
	sb::DrawBatch batch;
	sb::Stopwatch sw;

	sb::Triangle triangle1;
	sb::Triangle triangle2;
	sb::Quad quad1;
	
	triangle1.setPosition(-0.5f, 0.5f);
	triangle2.setPosition(0.5f, 0.5f);
	quad1.setPosition(0.5f, -0.5f);

	while (window.isOpen()) {
		sw.reset();

		window.update();
		window.clear();

		sb::Renderer::resetStatistics();

		batch.begin(window);
			batch.draw(triangle1);
			batch.draw(triangle2);
			batch.draw(quad1);
		batch.end();

		window.display();

		static std::size_t counter = 0;
		counter++;
		if (counter % 60 == 0) {
			SDL_Log("Num draw calls: %d", sb::Renderer::getNumDrawCalls());
			SDL_Log("Elapsed: %f, FPS: %f", sw.getElapsedMs(), 1000 / sw.getElapsedMs());
		}
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	demo1();
}
