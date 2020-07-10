#include "Demo.h"
#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include <iostream>

namespace demo
{
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

	void run()
	{
		sb::Window window;

		while (window.isOpen()) {
			sb::Input::update();
			window.update();
			window.clear(sb::Color(1, 1, 1, 1));
			window.display();
		}
	}
}