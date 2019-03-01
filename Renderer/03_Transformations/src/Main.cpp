#include "Window.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Triangle.h"
#include "Quad.h"
#include "Stopwatch.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>
#include <math.h>

void demo3()
{
	sb::Window window;

	sb::Triangle triangle;

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(triangle);
		window.display();
	}
}

float oscillate(float t, float length)
{
	double intpart;
	float remainder = (float)modf(t, &intpart);
	bool even = (int)intpart % 2 == 0;

	return even ? remainder : length - remainder;
}

void update2(sb::Triangle& triangle)
{
	static sb::Stopwatch sw;
	float t = sw.getElapsedSeconds();
	triangle.setRotation(t);
	triangle.setPosition(2 * oscillate(t / 2, 1) - 1, 2 * oscillate(t / 3, 1) - 1);
	triangle.setScale(oscillate(t / 5, 1), 0.5f * oscillate(t / 4, 1));
}

void demo2()
{
	sb::Window window;

	sb::Triangle triangle;

	while (window.isOpen()) {
		window.update();
		update2(triangle);
		window.clear();
		window.draw(triangle);
		window.display();
	}
}

void demo1() 
{
	sb::Window window;

	sb::Triangle triangle;
	triangle.setPosition(0.5f, 0.5f);
	triangle.setRotation(45 * sb::Transform::ToRadians);
	triangle.setScale(0.5f, 1.5f);

	sb::Quad quad;
	quad.setPosition(-0.25f, -0.5f);
	quad.setRotation(-10 * sb::Transform::ToRadians);
	quad.setScale(2, 0.25f);

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(triangle);
		window.draw(quad);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo3();
	
	// demo2();

	// demo1();
}
