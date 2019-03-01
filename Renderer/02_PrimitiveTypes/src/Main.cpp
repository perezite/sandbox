#include "Window.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Triangle.h"
#include "Quad.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>


void demo5() 
{
	sb::Window window;

	sb::Triangle triangle;
	sb::Quad quad;

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(quad);
		window.draw(triangle);
		window.display();
	}
}


float randomValue()
{
	float rnd = float(rand()) / float(RAND_MAX);
	return rnd;
}

void translate(sb::Vector2f& point, sb::Vector2f translation)
{
	float x = point.x + translation.x;
	float y = point.y + translation.y;
	point = sb::Vector2f(x, y);
}

void scale(sb::Vector2f& point, sb::Vector2f scaling)
{
	float x = point.x * scaling.x;
	float y = point.y * scaling.y;
	point = sb::Vector2f(x, y);
}

void rotate(sb::Vector2f& point, float radians) 
{
	float x = point.x * cosf(radians) - point.y * sinf(radians);
	float y = point.x * sinf(radians) + point.y * cosf(radians);
	point = sb::Vector2f(x, y);
}

void transform(std::vector<sb::Vertex>& shape, sb::Vector2f scaling, float radians, sb::Vector2f position)
{
	for (unsigned int i = 0; i < shape.size(); i++) {
		sb::Vector2f& point = shape[i].position;
		scale(point, scaling);
		rotate(point, radians);
		translate(point, position);
	}
}

void addShape(std::vector<sb::Vertex>& drawables, std::vector<sb::Vertex>& shape, bool strip) 
{
	// scale
	sb::Vector2f scaling(0.01f + randomValue() * 0.5f, 0.01f + randomValue() * 0.5f);
	sb::Vector2f position(-1 + 2 * randomValue(), -1 + 2 * randomValue());
	float radians = randomValue() * 2.0f * 3.1415f;
	 transform(shape, scaling, radians, position);

	// set strip connectors
	if (strip) 
	{ 
		shape[0] = shape[1];
		shape[shape.size() - 1] = shape[shape.size() - 2];
	}

	drawables.insert(drawables.end(), shape.begin(), shape.end());
}

void addTriangle(std::vector<sb::Vertex>& drawables, bool strip) 
{
	std::vector<sb::Vertex> triangle;

	if (strip) {
		triangle = {
			sb::Vertex(),
			sb::Vertex(sb::Vector2f(-0.5, -0.5), sb::Color(1, 0, 0, 1)),
			sb::Vertex(sb::Vector2f(0.5, -0.5), sb::Color(0, 1, 0, 1)),
			sb::Vertex(sb::Vector2f(0,    0.5), sb::Color(0, 0, 1, 1)),
			sb::Vertex(),
		};
	}
	else {
		triangle = {
			sb::Vertex(sb::Vector2f(-0.5, -0.5), sb::Color(1, 0, 0, 1)),
			sb::Vertex(sb::Vector2f(0.5, -0.5), sb::Color(0, 1, 0, 1)),
			sb::Vertex(sb::Vector2f(0,    0.5), sb::Color(0, 0, 1, 1)),
		};
	}


	addShape(drawables, triangle, strip);
}

void addQuad(std::vector<sb::Vertex>& drawables, bool strip)
{
	std::vector<sb::Vertex> quad;
	if (strip) {
		quad = {
			sb::Vertex(),
			sb::Vertex(sb::Vector2f(-0.5, -0.5), sb::Color(1, 0, 0, 1)),
			sb::Vertex(sb::Vector2f(0.5, -0.5), sb::Color(0, 1, 0, 1)),
			sb::Vertex(sb::Vector2f(-0.5, 0.5), sb::Color(0, 0, 1, 1)),
			sb::Vertex(sb::Vector2f(0.5, 0.5), sb::Color(0, 1, 1, 1)),
			sb::Vertex(),
		};
	}
	else {
		quad = {
			sb::Vertex(sb::Vector2f(-0.5, -0.5), sb::Color(1, 0, 0, 1)),			// bl
			sb::Vertex(sb::Vector2f(0.5, -0.5), sb::Color(0, 1, 0, 1)),				// br
			sb::Vertex(sb::Vector2f(-0.5, 0.5), sb::Color(0, 0, 1, 1)),				// tl
			sb::Vertex(sb::Vector2f(0.5, -0.5), sb::Color(0, 1, 0, 1)),				// br
			sb::Vertex(sb::Vector2f(0.5, 0.5), sb::Color(0, 1, 1, 1)),				// tr
			sb::Vertex(sb::Vector2f(-0.5, 0.5), sb::Color(0, 0, 1, 1)),				// tl
		};
	}

	addShape(drawables, quad, strip);
}

void addDrawables(std::vector<sb::Vertex>& drawables, std::size_t amount, bool strip)
{
	for (std::size_t i = 0; i < amount; i++) {
		if (rand() % 2 == 0)
			addTriangle(drawables, strip);
		else
			addQuad(drawables, strip);
	}
}

void demo4()
{
	sb::Window window;

	std::vector<sb::Vertex> drawables;
	addDrawables(drawables, 10, false);

	std::vector<sb::Vertex> stripDrawables;
	addDrawables(stripDrawables, 10, true);

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(drawables, sb::PrimitiveType::Triangles);
		window.draw(stripDrawables, sb::PrimitiveType::TriangleStrip);
		window.display();
	}
}

void demo3()
{
	sb::Window window;

	std::vector<sb::Vertex> drawables = {
		sb::Vertex(sb::Vector2f(-0.5f , -0.5f),  sb::Color(1, 0, 0, 1)),
		sb::Vertex(sb::Vector2f(    0,  -0.5f),  sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f(-0.25f,     0),  sb::Color(0, 0, 1, 1)),
		sb::Vertex(sb::Vector2f(-0.25f,     0),  sb::Color(0, 0, 1, 1)),			// degenerate
		sb::Vertex(sb::Vector2f(0.5f,    0.5f),  sb::Color(1, 0, 0, 1 )),			// degenerate
		sb::Vertex(sb::Vector2f(0.5f,    0.5f),  sb::Color(1, 0, 0, 1)),
		sb::Vertex(sb::Vector2f(0.75f,   0.5f),  sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f(0.5f,    0.75f), sb::Color(0, 0, 1, 1)),
		sb::Vertex(sb::Vector2f(0.75f,   0.75),  sb::Color(0, 1, 1, 1)),
	};

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(drawables, sb::PrimitiveType::TriangleStrip);
		window.display();
	}
}

void demo2()
{
	sb::Window window;

	std::vector<sb::Vertex> quad = {
		sb::Vertex(sb::Vector2f(-0.5, -0.5), sb::Color(1, 0, 0, 1)),
		sb::Vertex(sb::Vector2f( 0.5, -0.5), sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f(-0.5,  0.5), sb::Color(0, 0, 1, 1)),
		sb::Vertex(sb::Vector2f( 0.5,  0.5), sb::Color(0, 1, 1, 1)),
	};

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(quad, sb::PrimitiveType::TriangleStrip);
		window.display();
	}
}

void demo1()
{
	sb::Window window;

	std::vector<sb::Vertex> triangle = { 
		sb::Vertex(sb::Vector2f(-0.5, -0.5), sb::Color(1, 0, 0, 1)),
		sb::Vertex(sb::Vector2f( 0.5, -0.5), sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f( 0,    0.5), sb::Color(0, 0, 1, 1)),
	};

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(triangle, sb::PrimitiveType::TriangleStrip);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	demo5();

	// demo4();

	// demo3();

	// demo2();

	// demo1();
}
