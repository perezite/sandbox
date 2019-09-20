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
		//quad.setScale(.1f);
		triangle.setScale(.1f);
		// Triangle& triangle2 = triangle.createChild<Triangle>();

		//scene.findNode<Quad>()->setScale(.5f);
		
		// auto node = scene.findNode<Triangle>();
		// triangle.setScale(.1f);
		//auto triangles = scene.findNodes<Triangle>();
		//for (size_t i = 0; i < triangles.size(); i++) {
		//	auto triangle = *triangles[i];
		//	triangle.setDrawLayer(i);
		//	triangle.setScale(1 - float(i) * 0.3f);
		//}

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