#include "Demo.h"
#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include "Sprite.h"
#include "ParticleSystem.h"

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

	void run()
	{
		demo0();
	}
}

// TODO
// Add emitter as child of sprite
// Create sprite from scene
// Draw scene
// Implement getGlobalBounds for sprite
// Draw the sprite's global bounds
// Implement setGlobalTransform for emitter
// Draw the global coordinates as little dots
// Position emitter along the bottom of the sprite's bounds