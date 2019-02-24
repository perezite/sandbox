#include "Window.h"
#include "Triangle.h"
#include "Quad.h"
#include "Vertex.h"
#include <SDL2/SDL.h>
#include <vector>

void demo4()
{
	sb::Window window;

	std::vector<sb::Vertex> drawables = {
		// triangle
		sb::Vertex(sb::Vector2f(   -1, -1), sb::Color(1, 0, 0, 1)),
		sb::Vertex(sb::Vector2f(    0, -1), sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f(-0.5f,  0), sb::Color(0, 0, 1, 1)),

		// rectangle
		sb::Vertex(sb::Vector2f(    0,     0),	sb::Color(1, 0, 0, 1)),
		sb::Vertex(sb::Vector2f(	1,	   0),	sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f(	0,	   1),	sb::Color(0, 0, 1, 1)),
		sb::Vertex(sb::Vector2f(	1,	   0),	sb::Color(0, 1, 0, 1)),
		sb::Vertex(sb::Vector2f(	1,	   1),	sb::Color(0, 1, 1, 1)),
		sb::Vertex(sb::Vector2f(	0,	   1),	sb::Color(0, 0, 1, 1))
	};;

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(drawables);
		window.display();
	}
}

void demo3()
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

void demo2()
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
		window.draw(triangle);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	demo4();

	// demo3();

	// demo2();

	// demo1();

}
