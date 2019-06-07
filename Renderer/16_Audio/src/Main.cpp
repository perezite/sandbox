#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "Stopwatch.h"
#include "Math.h"
#include "Tween.h"
#include <iostream>
#include <vector>		
#include <algorithm>
#include <math.h>
#include <time.h>

float getSeconds() {
	static sb::Stopwatch sw;
	return sw.getElapsedSeconds();
}

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	float elapsed = getSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Audio - Build: " << configuration << ", " << __DATE__ << ", " << __TIME__);
}

void demo0() {
	sb::Window window;
	sb::Quad quad;

	window.getCamera().setWidth(1.5f);
	sb::Tween tween = sb::Tween().bounceOut(-0.4f, 0.4f, 2).quintInOut(0.4f, -0.5f, 3);
	quad.setScale(0.3f);

	while (window.isOpen()) {
		float t = getSeconds();
		sb::Input::update();
		window.update();
		sb::Vector2f position = sb::Vector2f(tween.value(t), tween.value(t));
		quad.setPosition(position);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

int main() {
	version();

	demo0();

	return 0;
}
