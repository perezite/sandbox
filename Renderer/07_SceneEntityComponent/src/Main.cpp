#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Demo1Scene.h"
#include "Demo2Scene.h"
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

void demo2() {
	sb::Window window;
	sb::Demo1Scene scene;
	
	while (scene.isPlaying()) {
		sb::Input::update();
		window.update();
		scene.update();

		window.clear();
		scene.draw(window);

		window.display();
	}
}

void runScene(sb::Scene& scene, sb::Window& window) {
	while (scene.isPlaying()) {
		sb::Input::update();
		window.update();
		scene.update();

		window.clear();
		scene.draw(window);

		window.display();
	}
}

void scene1(sb::Window& window) {
	sb::Demo1Scene scene;
	runScene(scene, window);
}

void scene2(sb::Window& window) {
	sb::Demo2Scene scene;
	runScene(scene, window);
}

void demo3() {
	SDL_Log("Press enter to switch scene");
	sb::Window window;

	while (true) {
		scene1(window);
		scene2(window);
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Entity Component Scene Renderer: Build %s %s", __DATE__, __TIME__);

	demo3();

	//demo2();

	//demo1();

	//demo0();
}
