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

	Vector2f& operator+=(Vector2f& left, const Vector2f& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	struct Transform {
		Vector2f position;
	};


	Transform& operator*=(Transform& left, const Transform& right)
	{		
		left.position += right.position;
		return left;
	}

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
		virtual void draw(DrawTarget& target, DrawState state) = 0;
	};

	class DrawTarget {
	public:
		virtual void draw(Mesh& mesh, const DrawState &state = DrawState()) = 0;

		void DrawTarget::draw(Drawable& drawable, const DrawState& state) {
			drawable.draw(*this, state);
		}
	};

	class Window : public DrawTarget {
	public:
		virtual void draw(Mesh& mesh, const DrawState &state = DrawState()) {
			std::cout << "Window::draw()" << std::endl;
		}
	};

	class DrawBatch : public DrawTarget {
		size_t _capacity;
	public:
		DrawBatch(size_t capacity = 512) 
			: _capacity(capacity)
		{ }

		virtual void draw(Mesh& mesh, const DrawState &state = DrawState()) {
		}
	};

	struct Transformable {
		Transform transform;
	};

	class Scene;
	class Node : public Drawable, public Transformable {
	public:
		virtual void update(Scene& scene) { };
		virtual void draw(DrawTarget& target, DrawState state = DrawState()) { }
	};

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

		~Scene() {
			for (size_t i = _nodes.size() - 1; i >= 0; i--)
				delete _nodes[i];
		}

		template <class T>
		T& create() {
			T* node = new T();
			_nodes.push_back(node);
			return *node;
		}

		virtual void draw(Mesh& mesh, const DrawState &state = DrawState()) { }
		
		virtual void draw(DrawTarget& target, DrawState state = DrawState()) { }
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

		virtual void draw(DrawTarget& target, DrawState state = DrawState()) {
			state.transform *= transform;
			target.draw(_mesh);
		}
	};

	class Triangle : public Node {
		Mesh _mesh;
	public:
		Triangle()
			: _mesh({
			Vertex(Vector2f(-.5f, -.5f)),
			Vertex(Vector2f(+.5f, -.5f)),
			Vertex(Vector2f( .0f, +.5f)) })
		{ }

		virtual void draw(DrawTarget& target, DrawState state = DrawState()) {
			state.transform *= transform;
			target.draw(_mesh);
		}
	};

	void demo1() {
		Window window;
		Scene scene;
		
		Quad& quad = scene.create<Quad>();
		Triangle& triangle = scene.create<Triangle>();

		/*while (window.isOpen()) {
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