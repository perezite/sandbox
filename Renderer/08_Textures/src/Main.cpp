#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>


void demo0() {
	sb::Window window;
	sb::Quad quad;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		
		window.clear();
		window.draw(quad);

		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Texture Renderer: Build %s %s", __DATE__, __TIME__);

	demo0();
}
