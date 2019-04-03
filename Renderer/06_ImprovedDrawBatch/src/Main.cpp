#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "HierarchyTester.h"
#include "DrawBatch.h"
#include "Renderer.h"
#include "Stopwatch.h"
#include "Math.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>

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

void printStats1()
{
	static std::size_t counter = 0;

	if (counter % 100 == 0)
		SDL_Log("%d", sb::Renderer::getNumDrawCalls());

	sb::Renderer::resetStatistics();
	counter++;	
}

void demo1() 
{
	sb::Window window;
	sb::DrawBatch batch;
	
	sb::HierarchyTester tester;
	tester.setScale(0.3f, 0.3f);

	while (window.isOpen()) {
		window.update();
		window.clear();
		batch.draw(tester);
		window.draw(batch);
		window.display();

		printStats1();
	}
}

void init2(sb::Drawable* drawable) {
	drawable->setPosition(sb::random(-1, 1), sb::random(-1, 1));
	drawable->setRotation(sb::random(2 * sb::Pi));
	drawable->setScale(sb::random(0.2f), sb::random(0.2f));
}

template <class T>
void init2(std::vector<sb::Drawable*>& drawables, std::size_t numShapes) {
	for (std::size_t i = 0; i < numShapes; i++) {
		sb::Drawable* drawable = (sb::Drawable*)new T();
		init2(drawable);
		drawables.push_back(drawable);
	}
}

void cleanup2(std::vector<sb::Drawable*> drawables) {
	for (std::size_t i = 0; i < drawables.size(); i++)
		delete drawables[i];

	drawables.clear();
}

void printStats2() {
	static std::size_t counter = 0;
	static sb::Stopwatch sw;

	counter++;
	if (counter % 100 == 0) {
		SDL_Log("Elapsed: %f, FPS: %f, DrawCalls: %d", 
			sw.getElapsedMs(), 1000 / sw.getElapsedMs(), sb::Renderer::getNumDrawCalls());
	}

	sw.reset();
	sb::Renderer::resetStatistics();
}

void demo2()
{
	sb::Window window;
	sb::DrawBatch batch(16384);

	std::vector<sb::Drawable*> drawables;
	init2<sb::Triangle>(drawables, 2500);
	init2<sb::Quad>(drawables, 2500);

	while (window.isOpen()) {
		window.update();
		window.clear();

		for (std::size_t i = 0; i < drawables.size(); i++)
			batch.draw(*drawables[i]);
		window.draw(batch);

		window.display();

		printStats2();	
	}

	cleanup2(drawables);
}

class Created {
private:
	std::size_t _val;
public:
	Created(std::size_t val) 
		: _val(val)
	{
		std::cout << "Created::Created()" << std::endl;
	}

	void print() {
		std::cout << _val << std::endl;
	}
};

class Creator {
private:
	std::vector<Created*> _created;
public:
	void add(Created* created) {
		_created.push_back(created);
	}
	void print() {
		for (std::size_t i = 0; i < _created.size(); i++)
			_created[i]->print();
	}
};

void demo3()
{
	Creator creator;
	creator.add(&Created(42));
	creator.add(&Created(43));
	creator.add(&Created(44));
	creator.print();
}

class MyValue {
	std::size_t _myVal;
public:
	MyValue(std::size_t myVal)
		: _myVal(myVal)
	{ }
};

std

class MyStuff {
public:
	MyStuff(int* myInt, MyValue& myValue) {

	}
};

template <class Type, class Arg1, class Arg2>
Type* create(Arg1 arg1, Arg2 arg2)
{
	return new Type(arg1, arg2);
}

void demo4() {
	int myInt = 42;
	int* myIntPointer = &myInt;
	MyValue myValue(43);
	MyStuff* stuff = create<MyStuff>(myIntPointer, myValue);
}

int main(int argc, char* args[])
{
	SDL_Log("DrawBatch Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo4();

	// demo3();

	// demo2();

	// demo1();

	// demo0();
}
