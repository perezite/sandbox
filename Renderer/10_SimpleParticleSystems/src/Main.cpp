#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include "Triangle.h"
#include "Texture.h"
#include "Sprite.h"
#include "Math.h"

class Scene2 {
	sb::DrawBatch batch;
	sb::Texture texture;
	std::vector<sb::Drawable*> drawables;

protected:
	void allocate() {
		drawables[0] = new sb::Quad();
		drawables[1] = new sb::Quad();
		drawables[2] = new sb::Triangle();
		drawables[3] = new sb::Triangle();
		drawables[4] = new sb::Sprite();
		drawables[5] = new sb::Sprite();

	}

	void init() {
		texture.loadFromAsset("Textures/GreenBlock.png");

		drawables[0]->setPosition(-0.75, -0.75f);
		drawables[0]->setScale(0.1f, 0.1f);
		drawables[0]->setRotation(-30 * sb::ToRadian);

		drawables[1]->setPosition(-0.5, -0.5f);
		drawables[1]->setScale(0.2f, 0.2f);
		drawables[1]->setRotation(30 * sb::ToRadian);

		drawables[2]->setPosition(0.5, 0.5f);
		drawables[2]->setScale(0.1f, 0.1f);
		drawables[2]->setRotation(-30 * sb::ToRadian);

		drawables[3]->setPosition(0.75, 0.75f);
		drawables[3]->setScale(0.2f, 0.2f);
		drawables[3]->setRotation(30 * sb::ToRadian);

		drawables[4]->setPosition(0.5, -0.5f);
		drawables[4]->setScale(0.1f, 0.1f);
		drawables[4]->setRotation(-30 * sb::ToRadian);
		((sb::Sprite*)drawables[4])->setTexture(&texture);

		drawables[5]->setPosition(0.75, -0.75f);
		drawables[5]->setScale(0.2f, 0.2f);
		drawables[5]->setRotation(30 * sb::ToRadian);
		((sb::Sprite*)drawables[5])->setTexture(&texture);
	}
	
public:
	Scene2()
		: drawables(6)
	{
		allocate();
		init();
	}

	void draw(sb::DrawTarget& target) {
		for (std::size_t i = 0; i < drawables.size(); i++)
			target.draw(drawables[i]);
	}


};

void demo2() {
	sb::Window window;
	Scene2 scene;

	while(window.isOpen()) {
		sb::Input::update();
		window.update();
		
		window.clear();
		scene.draw(window);
		window.display();
	}
}

void demo1() {
	sb::Window window;
	sb::Quad quad;
	quad.setScale(0.5f, 0.5f);
	quad.setRotation(-45 * sb::ToRadian);
	quad.setPosition(0.5f, 0.5f);

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

	demo2();

	// demo1();
}
