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

float computeQuintInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
	t = t - 2;
	return c / 2 * (t * t * t * t * t + 2) + b;
}

float computeLinearInOut(float t, float b, float c, float d) {
	return c * t / d + b;
}

inline float bounceOut(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeBounceOut(time - t0, from, to - from, duration);
}

inline float quintInOut(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeQuintInOut(time - t0, from, to - from, duration);
}

inline float linearInOut(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeLinearInOut(time - t0, from, to - from, duration);
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
	
	window.getCamera().setWidth(1.5);
	quad.setScale(0.3f);

	while (window.isOpen()) {
		float t = getSeconds();
		sb::Input::update();
		window.update();
		float position;
		if (t < 2)
			position = bounceOut(t, 1, 2, -0.4f, 0.4f);
		else
			position = quintInOut(t, 2.5f, 4.5f, 0.4f, -0.5f);
		quad.setPosition(position, position);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

class Tween {
	typedef float(*easingFunction)(float t, float t0, float t1, float from, float to);

	struct EasingCommand {
		float t0;
		float t1;
		float from;
		float to;
		easingFunction easing;

		EasingCommand() { }

		EasingCommand(float t0_, float t1_, float from_, float to_, easingFunction easing_)
			: t0(t0_), t1(t1_), from(from_), to(to_), easing(easing_)
		{ }
	};

	std::vector<EasingCommand> _easingCommands;

protected:
	EasingCommand getCurrentEasingCommand(float t) {
		for (std::size_t i = 0; i < _easingCommands.size(); i++) {
			if (t >= _easingCommands[i].t0 && t < _easingCommands[i].t1)
				return _easingCommands[i];
		}

		SB_ERROR("this should never happend");
		return EasingCommand();
	}

	template <easingFunction F>
	inline void addEasing(float from, float to, float duration) {
		float t0 = _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
		float t1 = t0 + duration;
		_easingCommands.push_back(EasingCommand(t0, t1, from, to, F));
	}

public:
	Tween()
	{ }

	float value(float t) {
		if (_easingCommands.empty())
			return 0;

		if (t < _easingCommands[0].t0)
			return _easingCommands[0].from;

		if (t >= _easingCommands[_easingCommands.size() - 1].t1)
			return _easingCommands[_easingCommands.size() - 1].to;

		EasingCommand easingCommand = getCurrentEasingCommand(t);
		return easingCommand.easing(t, easingCommand.t0, easingCommand.t1, easingCommand.from, easingCommand.to);
	}

	Tween& bounceOut(float from, float to, float duration) {
		addEasing<::bounceOut>(from, to, duration);
		return *this;
	}

	Tween& quintInOut(float from, float to, float duration) {
		addEasing<::quintInOut>(from, to, duration);
		return *this;
	}
};

void concept1() {
	sb::Window window;
	sb::Quad quad;

	window.getCamera().setWidth(1.5f);
	Tween tween = Tween().bounceOut(-0.4f, 0.4f, 2).quintInOut(0.4f, -0.5f, 3);
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

	 concept1();

	//concept0();

	std::cin.get();
	return 0;
}