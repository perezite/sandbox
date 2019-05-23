#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "Stopwatch.h"
#include "Math.h"
#include <iostream>
#include <vector>		
#include <algorithm>

float getSeconds() {
	static sb::Stopwatch sw;
	return sw.getElapsedSeconds();
}

float computeBounceOut(float t, float b, float c, float d) {
	if ((t /= d) < (1 / 2.75f)) {
		return c * (7.5625f * t * t) + b;
	}
	else if (t < 2 / 2.75f) {
		float postFix = t -= (1.5f / 2.75f);
		return c * (7.5625f * postFix * t + .75f) + b;
	}
	else if (t < 2.5 / 2.75) {
		float postFix = t -= (2.25f / 2.75f);
		return c * (7.5625f * postFix * t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c * (7.5625f * postFix * t + .984375f) + b;
	}
}

inline float bounceOut(float t, float duration, float from, float to) {
	return computeBounceOut(std::min(t, duration), from, to - from, duration);
}

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	float elapsed = getSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void concept0() {
	sb::Window window;
	sb::Quad quad;
	
	quad.setScale(0.3f);

	while (window.isOpen()) {
		float t = getSeconds();
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		float position = bounceOut(t, 2, -0.4f, 0.4f);
		quad.setPosition(position, position);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

void concept1() {
	sb::Window window;
	sb::Quad quad;

	// sb::Tween tween = sb::Tween::bounceIn(p1 = 20, p2 = 30, duration = 2).linearOut(p1 = 5, p2 = 12, duration = 0.5f);
	quad.setScale(0.3f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		// quad.setPosition(sb::Vector2f(tween[0.1f], tween[0.1f]));
		// quad.setPosition(sb::Vector2f(tween.value(0.1f), tween.value(0.1f));

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

class ParticleSystem {
	sb::Vector2f _speedRange;
	sb::Vector2f _rotationRange;

public:
	inline void setSpeedRange(float min, float max) { _speedRange = sb::Vector2f(min, max); }

	inline void setRotationRange(float min, float max) { _rotationRange = sb::Vector2f(min, max); }

	void update() { }

	void draw() { }
};

void initParticleSystem2(ParticleSystem& system) {
	system.setRotationRange(0, 360 * sb::ToRadian);
	system.setSpeedRange(1, 1.5);
}

void concept2() {
	sb::Window window;
	ParticleSystem particleSystem;
	initParticleSystem2(particleSystem);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		particleSystem.update();

		window.clear(sb::Color(1, 1, 1, 1));
		particleSystem.draw();
		window.display();
	}
}

int main() {
	// concept2();

	// concept1();

	concept0();

	std::cin.get();
	return 0;
}