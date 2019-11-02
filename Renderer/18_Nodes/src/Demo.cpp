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

		auto triangles = scene.findAll<Triangle>();
		auto quads = scene.findAll<Quad>();

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
			eraseFromMap(myMap, isEmpty);

			print(myMap);
		}

		std::cin.get();
	}

	void demo7() {
		Window window;
		Scene scene;

		auto& triangle = scene.create<Triangle>();
		auto& quad1 = triangle.createChild<Quad>();
		auto& quad2 = triangle.createChild<Quad>();
		auto& triangle2 = quad2.createChild<Triangle>();
		auto quads = scene.findAll<Quad>();

		triangle.setScale(.5f);
		quad1.setPosition(-.5f);
		quad1.setScale(.5f);
		quad1.setDrawLayer(1);
		quad2.setPosition(.5f);
		quad2.setScale(.5f);
		quad2.setDrawLayer(1);
		triangle2.setScale(.5f);
		triangle2.setDrawLayer(2);

		scene.enableDiagnostics(true);

		while (window.isOpen()) {
			Input::update();
			window.update();
			if (Input::isTouchGoingDown(1) && !quads.empty()) {
				scene.remove(*quads[0]);
				eraseFromVector(quads, quads[0]);
			}
			scene.update();

			window.clear();
			scene.draw(window);
			window.display();

			printRenderStatistics();
		}
	}

	BaseNode* getRandom(Scene& scene) {
		auto nodes = scene.getNodes();
		if (nodes.empty()) 
			nodes.push_back(&scene.create<EmptyNode>());

		return nodes[rand() % nodes.size()];
	}

	template <class T>
	void randomize(T& node) {
		node.setPosition(random(-.5f, .5f), random(-.5f, .5f));
		node.setScale(random(.1f, .5f));
		node.setRotation(random(0, 2) * Pi);
		node.setDrawLayer(rand() % 10);
	}

	template <class T>
	void createRandom(Scene& scene, BaseNode* parent) {
		randomize(parent->createChild<T>());
	}

	void createRandom(Scene& scene) {
		auto parent = getRandom(scene);

		if (rand() % 2 == 0)
			createRandom<Triangle>(scene, parent);
		else
			createRandom<Quad>(scene, parent);
	}

	void createRandom(Scene& scene, size_t count) {
		for (size_t i = 0; i < count; i++)
			createRandom(scene);
	}
	
	void eraseRandom(Scene& scene) {
		auto nodes = scene.getNodes();
		if (scene.getNodes().size() <= 1)
			return;

		scene.remove(*nodes[rand() % nodes.size()]);
	}

	void eraseRandom(Scene& scene, size_t count) {
		for (size_t i = 0; i < count; i++)
			eraseRandom(scene);
	}


	void demo8() {
		Window window;
		Scene scene;
		
		scene.enableDiagnostics(false);
		srand(42);

		auto count = 0;
		while (window.isOpen()) {
			Input::update();
			window.update();
			createRandom(scene, rand() % 5);
			if (scene.getNodes().size() > 100)
				eraseRandom(scene, rand() % 20);
			scene.update();

			window.clear();
			scene.draw(window);
			window.display();

			// printRenderStatistics();
		}

	}

	void runDemo() {
		demo8();
	}
}

// TODO
// implement update() recursively, just like draw()
// store the parent for each node and update the erase function accordigly
// Get rid of the deprecated draw() function in scene
// naming conventions: direct-child functions are named normally, recursive functions have the 'all' suffix
// final cleanup
