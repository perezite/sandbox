#include "Demo.h"
#include "Window.h"
#include "Quad.h"

using namespace sb;

namespace demo {

	class Scene {
	public:
	};

	void demo1() {
		Window window;
		/*Scene scene(window);
		Quad& quad = scene.create<Quad>();
		quad.setScale(.5f, .5f);
		quad.setDrawLayer(2);
		Triangle& childTriangle = quad.createChild<Triangle>();
		Triangle& triangle = scene.create<Triangle>();
		triangle.setScale(.1f, .1f);
		triangle.setPosition(.1f, -.1f);
		triangle.setDrawLayer(1);

		while (window.isOpen()) {
			window.update();
			scene.update();
			
			window.clear();
			window.draw(scene);
			window.display();
		}*/
	}

	void runDemo() {
		demo1();
	}
}