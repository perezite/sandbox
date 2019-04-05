#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Demo1Scene.h"
#include "Stickman.h"
#include "StickmanTester.h"
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

void demo1() {
	sb::Window window;
	sb::DrawBatch batch;
	sb::Stopwatch sw;

	sb::StickmanTester stickmanTester;
	stickmanTester.setScale(0.2f, 0.2f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		stickmanTester.update();

		window.clear();
		stickmanTester.setRotation(sw.getElapsedSeconds());
		stickmanTester.draw(window, batch);
		window.draw(batch);

		window.display();
	}
}

/*void demo2() {
	sb::Window window;
	sb::Demo1Scene scene;
	
	while (scene.isPlaying()) {
		sb::Input::update();
		window.update();

		scene.update();
		scene.draw(window);

		window.display();
	}
}*/

int main(int argc, char* args[])
{
	SDL_Log("Entity Component Scene Renderer: Build %s %s", __DATE__, __TIME__);

	// demo2();

	demo1();

	// demo0();
}
