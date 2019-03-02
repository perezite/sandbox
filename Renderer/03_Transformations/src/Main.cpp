#include "Window.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Triangle.h"
#include "Quad.h"
#include "Stopwatch.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <iostream>

float oscillate(float t, float length)
{
	double intpart;
	float remainder = (float)modf(t / length, &intpart);
	bool even = (int)intpart % 2 == 0;

	return even ? remainder * length : (1 - remainder) * length;
}

class SubIlluminatus : public sb::Drawable {
public:
	void update() {
		static sb::Stopwatch sw;
		float t = sw.getElapsedSeconds();
		setRotation(-5 * t);
		float vertical = 1 - 2 * oscillate(t, 1);
		setPosition(getPosition().x, vertical);
	}

	virtual void draw(sb::Window& window, sb::Transform transform) {
		transform *= getTransform();

		m_triangle.draw(window, transform);
	}

private:
	sb::Triangle m_triangle;
};

class Illuminatus : public sb::Drawable
{
public:
	void update() {
		static sb::Stopwatch sw;
		float t = oscillate(sw.getElapsedSeconds(), 2);
		setScale(t, 1);

		for (std::size_t i = 0; i < 3; i++)
			m_subIlluminati[i].update();
	}

	virtual void draw(sb::Window& window, sb::Transform transform) {
		transform *= getTransform();

		m_triangle.draw(window, transform);


		m_subIlluminati[0].draw(window, transform * sb::Transform(sb::Vector2f(-0.5f, -0.5f), sb::Vector2f(0.1f, 0.1f), 0));
		m_subIlluminati[1].draw(window, transform * sb::Transform(sb::Vector2f(0.5f, -0.5f), sb::Vector2f(0.1f, 0.1f), 0));
		m_subIlluminati[2].draw(window, transform * sb::Transform(sb::Vector2f(0.0f, 0.5f), sb::Vector2f(0.1f, 0.1f), 0));
	}

private:
	sb::Triangle m_triangle;

	SubIlluminatus m_subIlluminati[3];
};

void update3(Illuminatus& illuminatus) {
	static sb::Stopwatch sw;
	float t = sw.getElapsedSeconds();
	float angle = oscillate(t, 5);
	sb::Vector2f position(0.5f * cosf(angle), 0.5f * sinf(angle));
	illuminatus.setPosition(position);
	illuminatus.setRotation(t * 0.1f);
}

void demo3()
{
	sb::Window window;

	Illuminatus illuminatus;

	while (window.isOpen()) {
		window.update();
		update3(illuminatus);
		illuminatus.update();
		window.clear();
		window.draw(illuminatus);
		window.display();
	}
}

void update2(sb::Triangle& triangle)
{
	static sb::Stopwatch sw;
	float t = sw.getElapsedSeconds();
	triangle.setRotation(t);
	triangle.setPosition(2 * oscillate(t / 2, 1) - 1, 2 * oscillate(t / 3, 1) - 1);
	triangle.setScale(oscillate(t / 5, 1), 0.5f * oscillate(t / 4, 1));
}

void demo2()
{
	sb::Window window;

	sb::Triangle triangle;

	while (window.isOpen()) {
		window.update();
		update2(triangle);
		window.clear();
		window.draw(triangle);
		window.display();
	}
}

void demo1() 
{
	sb::Window window;

	sb::Triangle triangle;
	triangle.setPosition(0.5f, 0.5f);
	triangle.setRotation(45 * sb::Transform::ToRadians);
	triangle.setScale(0.5f, 1.5f);

	sb::Quad quad;
	quad.setPosition(-0.25f, -0.5f);
	quad.setRotation(-10 * sb::Transform::ToRadians);
	quad.setScale(2, 0.25f);

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(triangle);
		window.draw(quad);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo3();
	
	// demo2();

	// demo1();
}
