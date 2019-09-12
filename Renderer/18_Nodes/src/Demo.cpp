#include "Demo.h"
#include "Window.h"
#include "DrawBatch.h"
#include "Quad.h"

using namespace sb;

namespace demo {

	class Scene;
	class Node : public Drawable, public Transformable {
		std::vector<Node*> _children;
	public:
		virtual ~Node() {
			for (size_t i = 0; i < _children.size(); i++)
				delete _children[i];
		}
		std::vector<Node*> getChildren() const { return _children; }
		virtual void update(Scene& scene) { };
		virtual void draw(DrawTarget& target, sb::DrawState drawStates = DrawState::getDefault()) { }
		template <class T>
		T& createChild() {
			T* child = new T();
			_children.push_back(child);
			return *child;
		}
	};

	class Scene {
		DrawBatch _batch;
		size_t _capacity;
		size_t _drawableCount;
		std::vector<Node*> _nodes;
	public:
		Scene(ImmediateDrawTarget& target, size_t capacity = 8192)
			: _batch(target), _capacity(capacity), _drawableCount(0)
		{ }
		
		virtual ~Scene() {
			for (int i = _nodes.size() - 1; i >= 0; i--)
				delete _nodes[i];
		}
	};

	void demo1() {
		Window window;
		Scene scene(window);
		/*Quad& quad = scene.create<Quad>();
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