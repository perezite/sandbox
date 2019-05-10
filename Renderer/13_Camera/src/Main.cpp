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

void demo0() {
	sb::Window window(400, 600);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

int main(int argc, char* args[])
{
	demo0();

}
