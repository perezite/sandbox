#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Stickman.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

void demo0() {
	sb::Window window;
	sb::DrawBatch batch;

	sb::Stickman stickman;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		stickman.update();

		window.clear();
		stickman.draw(window, batch);
		window.draw(batch);

		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Entity Component Scene Renderer: Build %s %s", __DATE__, __TIME__);

	demo0();
}
