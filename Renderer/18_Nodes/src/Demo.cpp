#include "Demo.h"
#include "Window.h"
#include "Scene.h"
#include "Node.h"
#include "Quad.h"
#include "Triangle.h"
#include "Input.h"

using namespace sb;

namespace demo {
	void demo1() {
		Window window;
		Scene scene;

		Quad& quad = scene.create<Quad>();
		quad.setScale(.5f, .5f);
		quad.setDrawLayer(1);
		quad.createChild<Triangle>();
		Triangle& triangle = scene.create<Triangle>();
		triangle.setScale(.1f, .1f);
		triangle.setPosition(.1f, -.1f);
		triangle.setDrawLayer(2);

		while (window.isOpen()) {
			Input::update();
			window.update();
			scene.update();
			
			window.clear();
			scene.draw(window);
			window.display();
		}
	}

	void demo2() {
		Window window;
		Scene scene;

		Quad& quad = scene.create<Quad>();
		Triangle& triangle = quad.createChild<Triangle>();
		Triangle& triangle2 = triangle.createChild<Triangle>();
		triangle.setDrawLayer(1);
		triangle2.setDrawLayer(2);

		// scene.find<Quad>().setPosition(.8f);
		// auto triangles = scene.findMany<Triangle>();
		// for (size_t i = 0; i < triangles.size(); i++) {
		//		triangles[i]->setPosition(1 - float(i) * 0.1f);
		// }

		while (window.isOpen()) {
			Input::update();
			window.update();
			scene.update();

			window.clear();
			scene.draw(window);
			window.display();
		}
	}

	void runDemo() {
		demo2();
	}
}