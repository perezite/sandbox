#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
using namespace sb;

void run();
void logPerformance();

int main(int argc, char* args[])
{
	run();

	SDL_Log("Hierarchical Renderer (Text): Build %s %s", __DATE__, __TIME__);

	run();
}

float oscillate(float t, float length)
{
	double intpart;
	float remainder = (float)modf(t, &intpart);
	bool even = (int)intpart % 2 == 0;

	return even ? remainder : length - remainder;
}

void updateRod1(Drawable& rod1)
{
	static Stopwatch sw;
	rod1.setPosition(-0.5f + oscillate(sw.getElapsedSeconds(), 1), rod1.getPosition().y);
	rod1.setRotation(sw.getElapsedSeconds() * 0.3f);
}

void updateRod2(Drawable& rod2)
{
	static Stopwatch sw;
	rod2.setPosition(rod2.getPosition().x, -0.5f + oscillate(0.5f * sw.getElapsedSeconds(), 1));
	//rod2.setRotation(sw.getElapsedSeconds() * 0.3f);
}

void run()
{
	Window window;
	Stopwatch sw;

	sb::Rectangle rod1;
	rod1.setScale(0.05f, 1);

	sb::Rectangle rod2;
	rod2.setParent(rod1);
	rod2.setScale(20, 0.05f);

	//Triangle parent;
	//parent.setScale(0.2f, 0.2f);
	//parent.setPosition(0.5f, 0.5f);

	//sb::Rectangle child;
	//child.setScale(0.5f, 0.5f);
	//child.setPosition(0.7f, -0.7f);
	//child.setParent(parent);

	while (window.isOpen()) {
		window.update();

		 updateRod1(rod1);
		 updateRod2(rod2);
		// rod2.setRotation(sw.getElapsedSeconds());

		rod1.draw(window);
		rod2.draw(window);
		//8parent.setRotation(sw.getElapsedSeconds());
		//child.setRotation(-0.5f * sw.getElapsedSeconds());
		//parent.draw(window);
		//child.draw(window);
		window.display();
	}
}