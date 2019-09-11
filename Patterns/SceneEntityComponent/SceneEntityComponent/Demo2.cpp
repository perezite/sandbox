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

	Vector2f& operator+=(Vector2f& left, const Vector2f& right) {

		left.x += right.x;
		left.y += right.y;
		return left;
	}

	struct Transform {
		Vector2f position;
	};

	Transform& operator*=(Transform& left, const Transform& right) {		
		left.position += right.position;
		return left;
	}

	struct DrawState {
		size_t drawLayer;
		Transform transform;
	};

	const bool operator<(const DrawState& left, const DrawState& right) {
		return std::tie(left.drawLayer) < std::tie(right.drawLayer);
	}

	const bool canBatch(const DrawState& left, const DrawState& right) {
		 return std::tie() == std::tie();
	}

	struct Vertex {
		Vertex(const Vector2f position_ = Vector2f())
			: position(position_)
		{ }
		Vector2f position;
	};

	class Mesh {
		static bool Locked;
		std::vector<Vertex> _vertices;
	public:
		static void lock(bool locked) { Locked = locked; }
		static inline void assertUnlocked() {
			if (Locked)
				throw new std::exception("Modifying a mesh while rendering is not allowed.");
		}
		Mesh(std::vector<Vertex> vertices = std::vector<Vertex>())
			: _vertices(vertices)
		{ }
		const std::vector<Vertex>& getVertices() const { return _vertices; }
		std::vector<Vertex>& getVertices() { assertUnlocked(); return _vertices; }
	};

	bool Mesh::Locked = false;

	class DrawTarget;
	class Drawable {
	public:
		virtual void draw(DrawTarget& target, DrawState state) = 0;
	};

	class DrawTarget {
	public:
		virtual void draw(const Mesh& mesh, const DrawState &state = DrawState()) = 0;
		void draw(Drawable& drawable, const DrawState& state = DrawState()) {
			drawable.draw(*this, state);
		}
	};

	class ImmediateDrawTarget : public DrawTarget {
	public:
		virtual void drawImmediate(const std::vector<Vertex>& vertices, const DrawState& state) = 0;
	};

	class Window : public ImmediateDrawTarget {
	public:
		using DrawTarget::draw;
		virtual void draw(const Mesh& mesh, const DrawState &state = DrawState()) { drawImmediate(mesh.getVertices(), state); }
		virtual void drawImmediate(const std::vector<Vertex>& vertices, const DrawState& state) { 
			std::cout << "Window::drawImmediate() begin" << std::endl;
			for (size_t i = 0; i < vertices.size(); i++)
				std::cout << "x = " << vertices[i].position.x << ", y = " << vertices[i].position.y << std::endl;
			std::cout << "Window::drawImmediate() end" << std::endl;
		}
		bool isOpen() { return true; }
		void update() { }
		void clear() { }
		void display() { }
	};

	class DrawBatch  {
		ImmediateDrawTarget& _target;
		DrawState _currentState;
		std::vector<Vertex> _buffer;
	protected:
		bool mustFlush(const Mesh& mesh, const DrawState& state) {
			if (_buffer.empty())
				return false;
			if (!canBatch(state, _currentState))
				return true;
			if (_buffer.size() + mesh.getVertices().size() > _buffer.capacity())
				return true;
			return false;
		}
		void flush() {
			_target.drawImmediate(_buffer, _currentState);
			_buffer.clear();
		}
		void insert(const Mesh& mesh, const DrawState& state) {
			_buffer.insert(_buffer.end(), mesh.getVertices().begin(), mesh.getVertices().end());
			_currentState = state;
		}
	public:
		DrawBatch(ImmediateDrawTarget& target, size_t capacity = 512)
			: _target(target)
		{ 
			_buffer.reserve(capacity);
		}
		void draw(const Mesh& mesh, const DrawState &state = DrawState()) { 
			if (mustFlush(mesh, state))
				flush();
			insert(mesh, state);
		}
		virtual void draw() {
			if (!_buffer.empty())
				flush();
		}	
	};

	struct Transformable {
		Transform transform;
	};

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
		virtual void draw(DrawTarget& target, DrawState state = DrawState()) { }
		template <class T> 
		T& createChild() {
			T* child = new T();
			_children.push_back(child);
			return *child;
		}
	};

	class Scene : public DrawTarget, public Drawable {
		typedef std::vector<const Mesh*> Layer;
		typedef std::map<DrawState, Layer> LayerMap;
		LayerMap _layers;
		DrawBatch _batch;
		size_t _capacity;
		size_t _drawableCount;
		std::vector<Node*> _nodes;
	protected:
		bool mustFlush() {
			return _nodes.size() >= _capacity;
		}
		void flush() {
			for (LayerMap::iterator it = _layers.begin(); it != _layers.end(); it++)
				flush(it->second, it->first);	

			_batch.draw();
			_layers.clear();
		}
		void flush(const std::vector<const Mesh*> layer, const DrawState& state) {
			for (size_t i = 0; i < layer.size(); i++)
				_batch.draw(*layer[i], state);
		}
		void drawRecursively(Node& node, DrawState& state) {
			auto& children = node.getChildren();
			for (size_t i = 0; i < children.size(); i++) 
				drawRecursively((*children[i]), state);
			draw(node, state);
		}
	public:
		Scene(ImmediateDrawTarget& target, size_t capacity = 8192)
			: _batch(target), _capacity(capacity), _drawableCount(0)
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
		using DrawTarget::draw;
		virtual void draw(const Mesh& mesh, const DrawState &state = DrawState()) {
			_layers[state].push_back(&mesh);
			if (mustFlush())
				flush();
		}
		using Drawable::draw;
		virtual void draw(DrawTarget& target, DrawState state = DrawState()) {
			Mesh::lock(true);
			for (size_t i = 0; i < _nodes.size(); i++)
				drawRecursively(*_nodes[i], state);

			if (!_nodes.empty())
				flush();
			Mesh::lock(false);
		}
		void update() { 
			for (size_t i = 0; i < _nodes.size(); i++)
				_nodes[i]->update(*this);
		}
	};

	class Quad : public Node {
		Mesh _mesh;
		size_t _drawLayer;
	public:
		Quad() 
			: _mesh({ 
				Vertex(Vector2f(-.5f, -.5f)),
				Vertex(Vector2f(+.5f, -.5f)),
				Vertex(Vector2f(-.5f, +.5f)),
				Vertex(Vector2f(+.5f, +.5f))})
		{ }
		void setDrawLayer(size_t drawLayer) { _drawLayer = drawLayer; }
		virtual void draw(DrawTarget& target, DrawState state = DrawState()) {
			state.transform *= transform;
			state.drawLayer = _drawLayer;
			target.draw(_mesh, state);
		}
	};

	template <class T> 
	inline const T& toConst(const T& object) {
		return (const T&)object;
	}

	class Triangle : public Node {
		Mesh _mesh;
		size_t _drawLayer;
	public:
		Triangle()
			: _mesh({
			Vertex(Vector2f(-.2f, -.2f)),
			Vertex(Vector2f(+.2f, -.2f)),
			Vertex(Vector2f( .0f, +.2f)) })
		{ }
		void setDrawLayer(size_t drawLayer) { _drawLayer = drawLayer; }
		virtual void draw(DrawTarget& target, DrawState state = DrawState()) {
			state.transform *= transform;
			state.drawLayer = _drawLayer;
			target.draw(_mesh, state);
			auto& test = toConst(_mesh).getVertices();
		}
		void deform() {
			_mesh.getVertices()[2].position.y *= 1.5f;
		}
	};

	void init1(Scene& scene) {
		Quad& quad = scene.create<Quad>();
		quad.setDrawLayer(2);
		Triangle& childTriangle = quad.createChild<Triangle>();
		childTriangle.deform();
		childTriangle.setDrawLayer(3);
		Triangle& striangle = scene.create<Triangle>();
	}

	void demo1() {
		Window window;
		Scene scene(window);

		init1(scene);

		while (window.isOpen()) {
			std::cout << "main loop begin" << std::endl;
			window.update();
			scene.update();

			window.clear();
				
			window.draw(scene);
			window.display();
			std::cout << "main loop end" << std::endl;
			std::cin.get();
		}	
	}

	void run() {
		demo1();
	}
}