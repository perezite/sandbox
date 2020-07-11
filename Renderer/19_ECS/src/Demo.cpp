#include "Demo.h"
#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "Quad.h"
#include "Triangle.h"

using namespace sb;
using namespace std;

namespace demo
{
	float getSeconds() {
		static sb::Stopwatch sw;
		return sw.getElapsedSeconds();
	}

	float getDeltaSeconds()
	{
		static float lastElapsed = 0;
		float elapsed = getSeconds();
		float delta = elapsed - lastElapsed;
		lastElapsed = elapsed;
		return delta;
	}

	template <class T>
	void release(T& container) {
		for (typename T::iterator it = container.begin(); it != container.end(); it++)
			delete *it;
	}

	class Entity0 : public Drawable, public Transformable {
		vector<Entity0*> _children;

	public:
		
		virtual ~Entity0() {
			release(_children);
		}
	};

	class Sprite0 : public Entity0 {
		Sprite _sprite;

	public:
		Sprite0(sb::Texture& texture) {
			_sprite = Sprite(texture);
		}

		void setScale(float x, float y) { _sprite.setScale(x, y); }

		void setScale(float s) { setScale(s, s); }

		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault()) { _sprite.draw(window, states); }
	};

	class ParticleSystem0 : public Entity0 {
		ParticleSystem _emitter;
		
	public:
		ParticleSystem0() :_emitter(512) { 
			_emitter.setParticleColor(Color::createFromRGB(255, 0, 0));
			_emitter.setParticleSizeRange(.01f);
			_emitter.setEmissionRatePerSecond(5);
			_emitter.setParticleSpeedRange(.1f);
		}

		void update(float ds) { _emitter.update(ds); }

		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault()) { _emitter.draw(window, states); }

		void setPosition(float x, float y) { _emitter.setPosition(x, y); }
	};

	void demo0() {
		sb::Window window;
		sb::Texture texture("Textures/YellowBlock.png");
		Sprite0 sprite(texture);
		ParticleSystem0 emitter;
		sprite.setScale(.3f);
		emitter.setPosition(0, -.1f);

		while (window.isOpen()) {
			sb::Input::update();
			window.update();
			emitter.update(getDeltaSeconds());

			window.clear(sb::Color(1, 1, 1, 1));
			sprite.draw(window);
			emitter.draw(window);
			window.display();
		}
	}

	class Entity1 : public Drawable, public Transformable { 
		vector<Entity1*> _children;

	public:
		virtual ~Entity1() {
			release(_children);
		}

		vector<Entity1*>& getChildren() { return _children; }

		virtual void draw(DrawTarget& target, DrawStates states) = 0;

		template <class T>
		T& createChild() {
			T* entity = new T();
			_children.push_back(entity);
			return *entity;
		}

		void drawAll(DrawTarget& target, DrawStates states) {
			draw(target, states);

			states.transform *= getTransform();
			for (size_t i = 0; i < _children.size(); i++) 
				_children[i]->drawAll(target, states);
		}
	};

	class Quad1 : public Entity1 {
		Quad _quad;

	public:
		virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
			_quad.draw(target, states);
		}

		void setScale(float x, float y) { _quad.setScale(x, y); Transformable::setScale(x, y); }

		void setScale(float s) { setScale(s, s); }
	};

	void demo1() {
		sb::Window window;
		Quad1 quad;
		quad.setScale(.2f);

		while (window.isOpen()) {
			sb::Input::update();
			window.update();

			window.clear(sb::Color(1, 1, 1, 1));
			quad.draw(window);
			window.display();
		}
	}

	class Scene2 : public Entity1 { 
	public:
		virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
			states.transform *= getTransform();
			vector<Entity1*> children = getChildren();
			for (size_t i = 0; i < children.size(); i++) {
				children[i]->drawAll(target, states);
			}
		}
	};

	// Create quad entity from scene
	void demo2() {
		sb::Window window;
		Scene2 scene;
		Quad1& quad = scene.createChild<Quad1>();
		quad.setScale(.2f);

		while (window.isOpen()) {
			sb::Input::update();
			window.update();

			window.clear(sb::Color(1, 1, 1, 1));
			quad.draw(window);
			window.display();
		}
	}

	// Draw the scene
	void demo3() {
		Window window;
		Scene2 scene;
		Quad1& quad = scene.createChild<Quad1>();
		quad.setScale(.2f);

		while (window.isOpen()) {
			Input::update();
			window.update();

			window.clear(Color(1, 1, 1, 1));
			scene.draw(window);
			window.display();
		}
	}

	class Triangle4 : public Entity1 {
		Triangle _triangle;

	public:
		virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
			_triangle.draw(target, states);
		}

		void setScale(float x, float y) { _triangle.setScale(x, y); Transformable::setScale(x, y); }

		void setScale(float s) { setScale(s, s); }

		void setPosition(float x, float y) { _triangle.setPosition(x, y); Transformable::setPosition(x, y); }

		void setPosition(float pos) { setPosition(pos, pos); }
	};

	// Add triangle as child of quad
	void demo4() {
		Window window;
		Scene2 scene;
		Quad1& quad = scene.createChild<Quad1>();
		Triangle4& triangle = quad.createChild<Triangle4>();
		
		quad.setScale(.2f);
		triangle.setScale(.5f); triangle.setPosition(-.5f);

		while (window.isOpen()) {
			Input::update();
			window.update();

			window.clear(Color(1, 1, 1, 1));
			scene.draw(window);
			window.display();
		}
	}

	void run()
	{
		demo4();
		//demo3();
		//demo2();
		//demo1();
		//demo0();
	}
}

// TODO
// Implement getGlobalBounds for quad
// Draw the quad global bounds
// Implement setGlobalTransform for triangle
// Position triangle along the bottom of the quads 
// Add rotator component for the triangle
// Add deleter component for the triangle
// Listen for keypress in the delete. Delete self (from scene) when key is pressed: scene.dispose(this)
// Add scene.find<T>(entity) method to find single child entity or component of given entity
// Find triangle using scene.find<Triangle>(quad);
// Find rotator using scene.find<Rotator>(quad);
// Add sierpinski