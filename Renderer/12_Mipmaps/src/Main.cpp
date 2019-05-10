#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Logger.h"

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	static sb::Stopwatch sw;
	float elapsed = sw.getElapsedSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void update0(sb::Sprite& sprite, sb::Window& window, float ds) {
	const sb::Vector2f& scale = sprite.getScale();
	if (sb::Input::isTouchDown(1)) {
		const sb::Vector2f& touch = sb::Input::getTouchPosition(window);
		if (touch.y < 0 && touch.x >= 0)
			sprite.setScale((1 + ds) * scale);
		if (touch.y < 0 && touch.x < 0)
			sprite.setScale((1 - ds) * scale);
		if (touch.x >= 0 && touch.y > 0) {
			sprite.getTexture()->enableMipmaps(true);
			SB_MESSAGE("mipmaps enabled");
		}
		if (touch.x < 0 && touch.y > 0) {
			sprite.getTexture()->enableMipmaps(false);
			SB_MESSAGE("mipmaps disabled"); 
		}
	}
}

void demo0() {
	sb::Window window(400, 600);
	sb::Texture texture;
	texture.loadFromAsset("Textures/Checkerboard.png");

	sb::Sprite checkerboard;
	checkerboard.setScale(1, window.getAspect());
	checkerboard.setTexture(&texture);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		update0(checkerboard, window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(checkerboard);
		window.display();
	}
}

void demo1() {
	sb::Window window(400, 600);
	sb::Texture texture;
	texture.loadFromAsset("Textures/GreenBlock.png");

	sb::Sprite greenBlock;
	greenBlock.setScale(1, window.getAspect());
	greenBlock.setTexture(&texture);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		update0(greenBlock, window, ds);

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
