#include "Demo.h"
#include "Window.h"
#include "Scene.h"
#include "Node.h"
#include "Quad.h"
#include "Triangle.h"
#include "Input.h"
#include "Math.h"
#include "Enumerable.h"
#include "Memory.h"

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

	void demo3() {
		Window window;
		Scene scene;

		auto& shape1 = scene.create<Quad>();
		auto& shape2 = shape1.createChild<Triangle>();
		auto& shape3 = shape2.createChild<Triangle>();
		auto& shape4 = shape3.createChild<Quad>();

		shape1.setScale(.5f);
		shape2.setScale(.5f);
		shape2.setPosition(.5f);
		shape3.setScale(0.5f);
		shape3.setPosition(-.5f);
		shape4.setScale(.5f);
		shape4.setPosition(0, 0.5f);

		while (window.isOpen()) {
			Input::update();
			window.update();					
			scene.update();

			shape1.rotate(scene.getDeltaSeconds());
			shape2.rotate(-2 * scene.getDeltaSeconds());
			shape3.rotate(3 * scene.getDeltaSeconds());

			window.clear();
			scene.draw(window);
			window.display();

			printRenderStatistics();
		}
	}

	void demo4() {
		Window window;
		Scene scene;

		auto& shape1 = scene.create<Quad>();
		auto& shape2 = shape1.createChild<Triangle>();
		auto& shape3 = shape2.createChild<Triangle>();
		auto& shape4 = shape3.createChild<Quad>();

		shape1.setScale(.5f);
		shape1.setDrawLayer(0);
		shape2.setScale(.5f);
		shape2.setPosition(.5f);
		shape2.setDrawLayer(1);
		shape3.setScale(0.5f);
		shape3.setPosition(-.5f);
		shape3.setDrawLayer(2);
		shape4.setScale(.5f);
		shape4.setPosition(0, 0.5f);
		shape4.setDrawLayer(3);

		while (window.isOpen()) {
			Input::update();
			window.update();
			scene.update();

			shape1.rotate(scene.getDeltaSeconds());
			shape2.rotate(-2 * scene.getDeltaSeconds());
			shape3.rotate(3 * scene.getDeltaSeconds());

			window.clear();
			scene.draw(window);
			window.display();

			printRenderStatistics();
		}
	}

	void demo5() {
		Window window;
		Scene scene;

		auto& shape1 = scene.create<Quad>();
		auto& shape2 = shape1.createChild<Triangle>();
		auto& shape3 = shape2.createChild<Triangle>();
		auto& shape4 = shape3.createChild<Quad>();

		shape1.setScale(.5f);
		shape1.setDrawLayer(0);
		shape2.setScale(.5f);
		shape2.setPosition(.5f);
		shape2.setDrawLayer(1);
		shape3.setScale(0.5f);
		shape3.setPosition(-.5f);
		shape3.setDrawLayer(2);
		shape4.setScale(.5f);
		shape4.setPosition(0, 0.5f);
		shape4.setDrawLayer(3);

		auto triangles = scene.findMany<Triangle>();
		auto quads = scene.findMany<Quad>();

		while (window.isOpen()) {
			Input::update();
			window.update();
			scene.update();

			for (size_t i = 0; i < triangles.size(); i++) {
				int multiplier = (i + 1) * (i % 2 == 0 ? 1 : -1);
				triangles[i]->rotate(multiplier * 2 * scene.getDeltaSeconds());
			}

			for (size_t i = 0; i < quads.size(); i++) 
				quads[i]->setScale(0.1f + oscillate((i + 1) * .1f * scene.getSeconds(), .4f));

			window.clear();
			scene.draw(window);
			window.display();

			printRenderStatistics();
		}
	}

	void print(std::map<std::string, Enumerable<int>>& m) {
		for (auto it = m.begin(); it != m.end(); it++) {
			std::cout << it->first << ": ";
			for (size_t i = 0; i < it->second.size(); i++) {
				std::cout << i << " ";
			}

			std::cout << std::endl;
		}
	}

	void fill(std::map<std::string, Enumerable<int>>& m, const std::string& key, size_t count) {
		for (size_t i = 0; i < count; i++) {
			m[key].push_back(i);
		}
	}

	void clear(std::map<std::string, Enumerable<int>>& m, const std::string& key) {
		size_t count = m[key].size();
		for (size_t i = 0; i < count; i++) {
			m[key].pop_back();
		}
	}

	bool isEmpty(const std::string& key, const Enumerable<int>& val) {
		return val.empty();
	}

	void demo6() {
		{
			std::cout << "test 1" << std::endl;
			Enumerable<int> list;
			for (auto i = 0; i < 16; i++) {
				list.push_back(i);
				std::cout << list.size() << " " << list.capacity() << std::endl;
			}
		}

		{
			std::cout << "test 2" << std::endl;
			Enumerable<int> list(32);
			auto count = list.size();
			for (size_t i = 0; i < count; i++) {
				list.pop_back();
				list.adjust_capacity();
				std::cout << list.size() << " " << list.capacity() << std::endl;
			}
		}

		{
			std::cout << "test 3" << std::endl;
			std::map<std::string, Enumerable<int>> myMap;
			fill(myMap, "one", 16);
			fill(myMap, "two", 16);
			fill(myMap, "three", 16);
			clear(myMap, "two");
			deleteFromMap(myMap, isEmpty);

			print(myMap);
		}

		std::cin.get();
	}

	void demo7() {
		Window window;
		Scene scene;

		auto& triangle = scene.create<Triangle>();

		while (window.isOpen()) {
			Input::update();
			window.update();
			if (Input::isKeyGoingDown(KeyCode::d))
				scene.remove(triangle);
			scene.update();
			
			window.clear();
			scene.draw(window);
			window.display();

			printRenderStatistics();
		}
	}

	void runDemo() {
		demo7();
	}
}

// TODO
// Implement removal of nodes
// Make scene inherit from node (remove is overly complicated otherwise)
// Add printing of scene memory footprint
// Add a test with loads of fast removals and insertions
// implement update() recursively, just like draw()
// Get rid of the deprecated draw() function in scene
