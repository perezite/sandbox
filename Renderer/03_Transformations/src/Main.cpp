#include "Window.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Triangle.h"
#include "Quad.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>

void demo1() 
{
	sb::Window window;

	sb::Triangle triangle;
	triangle.setPosition(0.5f, 0.5f);
	triangle.setRotation(45 * sb::Transform::ToRadians);
	triangle.setScale(sb::Vector2f(0.5f, 1.5f));

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

	demo1();

}
