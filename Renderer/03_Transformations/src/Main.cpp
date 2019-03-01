#include "Window.h"
#include "Vertex.h"
#include "Triangle.h"
#include "PrimitiveType.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>

void demo1()
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

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
}
