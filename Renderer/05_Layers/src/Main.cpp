#include "Window.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Quad.h"
#include "RenderStates.h"  
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

void demo1() 
{
	sb::Window window;

	sb::Triangle triangle1;
	sb::Quad quad1;

	triangle1.setPosition(-0.1f, 0.1f);
	quad1.setPosition(0.1f, -0.1f);

	while (window.isOpen()) {
		window.update();
		
		window.clear();
		window.draw(triangle1);
		window.draw(quad1, 0);

		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Application: Build %s %s", __DATE__, __TIME__);

	demo1();
}
