#include "Window.h"
#include "Input.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>

void update1() {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Return))
		std::cout << "Enter going down" << std::endl;
	if (sb::Input::isKeyDown(sb::KeyCode::LeftControl))
		std::cout << "Left control pressed" << std::endl;
}

void demo1() 
{
	sb::Window window;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		update1();
		window.clear();
		window.display();
	}

}

int main(int argc, char* args[])
{
	SDL_Log("DrawBatch Renderer: Build %s %s", __DATE__, __TIME__);

	demo1();
}
