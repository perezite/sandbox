#include "Demo1.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>

namespace demo2
{
	struct Vector2f {
		Vector2f(float x_ = 0 ,float y_ = 0) 
			: x(x_), y(y_)
		{

		}
		float x, y;
	};

	struct Transform {
		Vector2f position;
	};

	struct DrawState {
		Transform transform;
	};

	struct Vertex {
		Vertex(const Vector2f position_)
			: position(position_)
		{ }
		Vector2f position;
	};

	struct Mesh {
		std::vector<Vertex> vertices;

		Mesh(std::vector<Vertex> vertices_ = std::vector<Vertex>())
			: vertices(vertices_)
		{ }
	};

	class DrawTarget;
	class Drawable {
	public:
		virtual void draw(DrawTarget& target, const DrawState state) = 0;
	};

	class DrawTarget {
	public:
		virtual void draw(Mesh& mesh, DrawState &state = DrawState()) = 0;

		void DrawTarget::draw(Drawable& drawable, const DrawState& state) {
			drawable.draw(*this, state);
		}
	};

	class Window : public DrawTarget {
	public:
		virtual void draw(Mesh& mesh, DrawState &state = DrawState()) {
			std::cout << "Window::draw()" << std::endl;
		}
	};

	class DrawBatch : public DrawTarget {
		size_t _capacity;
	public:
		DrawBatch(size_t capacity = 512) 
			: _capacity(capacity)
		{ }

		virtual void draw(Mesh& mesh, DrawState &state = DrawState()) {
		}
	};

	class Node;
	class Scene : public DrawTarget, public Drawable {
		typedef std::vector<Drawable*> Layer;
		typedef std::map<DrawState, Layer> LayerMap;
		LayerMap _layers;
		DrawBatch _batch;
		size_t _capacity;
		size_t _drawableCount;
		std::vector<Node*> _nodes;
	public:
		Scene(size_t capacity = 8192)
			: _capacity(capacity), _drawableCount(0)
		{ }

		virtual void draw(Mesh& mesh, DrawState &state = DrawState()) { }
		
		virtual void draw(DrawTarget& target, const DrawState state = DrawState()) { }
	};

	class Node : public Drawable {
	public:
		virtual void update(Scene& scene) { };
		virtual void draw(DrawTarget& target, const DrawState state = DrawState()) { }
	};

	class Quad : public Node {
		Mesh _mesh;
	public:
		Quad() 
			: _mesh({ 
				Vertex(Vector2f(-.5f, -.5f)),
				Vertex(Vector2f(+.5f, -.5f)),
				Vertex(Vector2f(-.5f, +.5f)),
				Vertex(Vector2f(+.5f, +.5f))})
		{ }

		virtual void draw(DrawTarget& target, const DrawState state = DrawState()) {

		}
	};

	void demo1() {

		Window window;
		Scene scene;

		/*
		Quad& quad = scene.create<Quad>();
		Triangle& triangle = quad.create<Triangle>();

		while (window.isOpen()) {
			window.update();
			scene.update();

			window.clear();
			window.draw(scene);
			
		}

		*/
	}

	void run() {
		demo1();
	}
}