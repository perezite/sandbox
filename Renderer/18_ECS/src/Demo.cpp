#include "Demo.h"
#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include "Sprite.h"
#include <iostream>

using namespace sb;

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

	class Sprite0 {
		Sprite _sprite;

	public:
		Sprite0(sb::Texture& texture) {
			_sprite = Sprite(texture);
		}

		void setScale(float x, float y) { _sprite.setScale(x, y); }

		void setScale(float s) { setScale(s, s); }

		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault()) { _sprite.draw(window, states); }
	};

	void demo0() {
		sb::Window window;
		sb::Texture texture("Textures/YellowBlock.png");
		Sprite0 sprite(texture);
		sprite.setScale(.3f);

		while (window.isOpen()) {
			sb::Input::update();
			window.update();

			window.clear(sb::Color(1, 1, 1, 1));
			sprite.draw(window);
			window.display();
		}
	}

	void run()
	{
		demo0();
	}
}