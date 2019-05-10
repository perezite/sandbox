#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Logger.h"
#include "Math.h"

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	static sb::Stopwatch sw;
	float elapsed = sw.getElapsedSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void demo0() {
	sb::Window window(300, 600);
	window.getCamera().setWidth(0.75f);

	sb::Texture texture;
	texture.enableMipmaps(true);
	texture.loadFromAsset("Textures/CoordinateSystem.png");

	sb::Sprite greenBlock;
	greenBlock.setTexture(&texture);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(greenBlock);
		window.display();
	}
}

void update1(sb::Window& window, float ds) {
	window.getCamera().rotate(ds);
}

void update1b(sb::Transformable& block, float ds) {
	block.rotate(ds);
}

void demo1() {
	sb::Window window(300, 600);

	sb::Texture texture;
	texture.enableMipmaps(true);
	texture.loadFromAsset("Textures/ControlQuad.png");

	sb::Sprite greenBlock;
	greenBlock.setTexture(&texture);
	greenBlock.setScale(0.5f, 1);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		update1(window, ds);
		// update1b(greenBlock, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(greenBlock);
		window.display();
	}
}

int main(int argc, char* args[])
{
	demo1();

	// demo0();
}
