#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include <iostream>

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
		if (sb::Input::getTouchPosition(window).x >= 0)
			sprite.setScale((1 + ds) * scale);
		else 
			sprite.setScale((1 - ds) * scale);
	}
}

void demo0() {
	sb::Window window(400, 600);
	sb::Texture texture;
	texture.loadFromAsset("Textures/Checkerboard.png");

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
	demo0();
}
