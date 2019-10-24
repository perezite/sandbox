#include "Demo.h"
#include "Window.h"
#include "Scene.h"
#include "Node.h"
#include "Quad.h"
#include "Triangle.h"
#include "Input.h"

using namespace sb;

namespace demo {
	void printRenderStatistics() {
		std::cout << "Draw calls: " << Renderer::getNumDrawCalls() << std::endl;
		Renderer::resetStatistics();
	}

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

		auto& shape1 = scene.create<Triangle>();
		auto& shape2 = scene.create<Triangle>();
		auto& shape3 = scene.create<Quad>();
		auto& shape4 = scene.create<Quad>();

		shape1.setPosition(-.25f);
		shape1.setScale(.3f);
		shape2.setPosition(.25f, -.25f);		
		shape2.setScale(.4f);
		shape3.setPosition(.25f);
		shape3.setScale(.2f);
		shape4.setPosition(-.25f, .25f);
		shape4.setScale(.1f);

		while (window.isOpen()) {
			Input::update();
			window.update();
			scene.update();

			window.clear();
			scene.draw(window);
			window.display();

			printRenderStatistics();
		}
	}

	void runDemo() {
		demo2();
	}
}