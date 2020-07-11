#include "Demo.h"
#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "Quad.h"

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

	class Entity1 { 
		vector<Entity1*> _children;

	public:
		virtual ~Entity1() {
			release(_children);
		}

		template <class T>
		T& createChild() {
			T* entity = new T();
			_children.push_back(entity);
			return *entity;
		}
	};

	class Quad1 : public Quad, public Entity1 {

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

	class Scene1 : public Entity1 { };

	// Create quad entity from scene
	void demo2() {
		sb::Window window;
		Scene1 scene;
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

	void run()
	{
		demo2();
		//demo1();
		//demo0();
	}
}

// TODO
// Draw scene
// Add triangle as child for quad
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