#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "SceneBuilder.h"
#include "Quad.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

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

void runScene2() {
	
}

/*template <class T>
void addShapes(SceneBuilder& builder, std::size_t numEntites, std::string entityName) {
	for (std::size_t i = 0; i < 1000; i++) {
		std::ostringstream name;
		name << entityName << i;
		builder.addEntity(os.str())
			.withComponent<ShapeDrawer>()
			.withPosition(Math::random(-1, 1))
			.withRotation(Math::random(2 * Math::Pi))
			.withScale(Math::random(0.2f, 0.5f), Math::random(0.2f, 0.5f));
	}
}*/


void scene2(sb::Window& window) {
	/*sb::Scene scene;
	SceneBuilder builder(scene);
	std::vector<Triangle> triangles(1000);
	std::vector<Quad> quads(1000);
	
	for (std::size_t i = 0; i < 

	*/

}

/*void runScene1(sb::Scene& scene, sb::Window& window) {
	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		scene.update();
		window.clear();
		// scene.draw(window);
		window.display();
	}
}
*/

void scene1(sb::Window& window) {
	sb::Scene scene;
	sb::Quad quad;

	sb::SceneBuilder(scene).addEntity("paddle");

	/*SceneBuilder(scene).addEntity("paddle")
		.withComponent<Paddle>()
		.withDrawable(quad);
			.withScale(0.5f, 0.1f)
		.withChildEntity("leftPropulsion)
			.withPosition(-0.25f, -0.1f)
			.withScale(0.1f, 0.1f)
			.withDrawable<Triangle>()
		
	runScene1(scene);*/
}

void demo2() {
	sb::Window window;
	scene1(window);
	scene2(window);
}

/*
void runScene3() {
}

void drawMario( ) {
}

struct Mario : Actor {
	Sprite torso;
	Sprite feet;

	void update() {
		...
	}
}

void demo3() 
{
	Window window;
	DrawBatch batch;

	Mario mario,

	while (window.isOpen()) {
		Input::update();
		window.update();

		batch.draw(mario.torso);
		batch.drwa(mario.feet);
		window.draw(batch);
		window.draw(marioFeet);
	}
}
*/

void demo3() 
{
	
}

int main(int argc, char* args[])
{
	SDL_Log("DrawBatch Renderer: Build %s %s", __DATE__, __TIME__);

	void demo3();

	// demo2();

	// demo1();
}
