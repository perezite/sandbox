#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "Stopwatch.h"
#include "Math.h"
#include "Body.h"
#include <iostream>
#include <vector>		
#include <algorithm>

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

float computeBounceOut(float t, float b, float c, float d) {
	if ((t /= d) < (1 / 2.75f)) {
		return c * (7.5625f * t * t) + b;
	}
	else if (t < 2 / 2.75f) {
		float postFix = t -= (1.5f / 2.75f);
		return c * (7.5625f * postFix * t + .75f) + b;
	}
	else if (t < 2.5 / 2.75) {
		float postFix = t -= (2.25f / 2.75f);
		return c * (7.5625f * postFix * t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c * (7.5625f * postFix * t + .984375f) + b;
	}
}

float computeQuintInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
	t = t - 2;
	return c / 2 * (t*t*t*t*t + 2) + b;
}

float computeLinearInOut(float t, float b, float c, float d) {
	return c * t / d + b;
}

inline float bounceOut(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeBounceOut(time - t0, from, to - from, duration);
}

inline float quintInOut(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeQuintInOut(time - t0, from, to - from, duration);
}

inline float linearInOut(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeLinearInOut(time - t0, from, to - from, duration);
}

void demo0() {
	sb::Window window;
	sb::Quad quad;
	
	window.getCamera().setWidth(1.5);
	quad.setScale(0.3f);

	while (window.isOpen()) {
		float t = getSeconds();
		sb::Input::update();
		window.update();
		float position;
		if (t < 2)
			position = bounceOut(t, 1, 2, -0.4f, 0.4f);
		else
			position = quintInOut(t, 2.5f, 4.5f, 0.4f, -0.5f);
		quad.setPosition(position, position);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

class Tween {
	typedef float(*easingFunction)(float t, float t0, float t1, float from, float to);

	struct EasingCommand {
		float t0;
		float t1;
		float from;
		float to;
		easingFunction easing;

		EasingCommand() { }

		EasingCommand(float t0_, float t1_, float from_, float to_, easingFunction easing_)
			: t0(t0_), t1(t1_), from(from_), to(to_), easing(easing_)
		{ }
	};

	std::vector<EasingCommand> _easingCommands;

protected:
	EasingCommand getCurrentEasingCommand(float t) {
		for (std::size_t i = 0; i < _easingCommands.size(); i++) {
			if (t >= _easingCommands[i].t0 && t < _easingCommands[i].t1)
				return _easingCommands[i];
		}

		SB_ERROR("this line must never be reached");
		return EasingCommand();
	}

	template <easingFunction F>
	inline void addEasing(float from, float to, float duration) {
		float t0 = _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
		float t1 = t0 + duration;
		_easingCommands.push_back(EasingCommand(t0, t1, from, to, F));
	}

public:
	Tween()
	{ }

	float value(float t) {
		if (_easingCommands.empty())
			return 0;

		if (t < _easingCommands[0].t0)
			return _easingCommands[0].from;

		if (t >= _easingCommands[_easingCommands.size() - 1].t1)
			return _easingCommands[_easingCommands.size() - 1].to;

		EasingCommand easingCommand = getCurrentEasingCommand(t);
		return easingCommand.easing(t, easingCommand.t0, easingCommand.t1, easingCommand.from, easingCommand.to);
	}

	float getDuration() {
		return _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
	}

	Tween& bounceOut(float from, float to, float duration) {
		addEasing<::bounceOut>(from, to, duration);
		return *this;
	}

	Tween& quintInOut(float from, float to, float duration) {
		addEasing<::quintInOut>(from, to, duration);
		return *this;
	}

	Tween& linearInOut(float from, float to, float duration) {
		addEasing<::linearInOut>(from, to, duration);
		return *this;
	}

	static Tween None(float value = 0) {
		return Tween().linearInOut(value, value, 0);
	}
};

void demo1() {
	sb::Window window;
	sb::Quad quad;

	window.getCamera().setWidth(1.5f);
	Tween tween = Tween().bounceOut(-0.4f, 0.4f, 2).quintInOut(0.4f, -0.5f, 3);
	quad.setScale(0.3f);

	while (window.isOpen()) {
		float t = getSeconds();
		sb::Input::update();
		window.update();
		sb::Vector2f position = sb::Vector2f(tween.value(t), tween.value(t));
		quad.setPosition(position);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

class ParticleSystem2 : public sb::Drawable, public sb::Transformable {
	struct Particle {
		float lifetime = 0;
		float timeToLive;
		sb::Vector2f velocity;
		sb::Vector2f startScale;
		float startRotation;
		sb::Quad quad;

		Particle(float timeToLive_, const sb::Vector2f& velocity_, const sb::Vector2f& startScale_, float startRotation_)
			: timeToLive(timeToLive_), velocity(velocity_), startScale(startScale_), startRotation(startRotation_)
		{ }
	};

	sb::Vector2f _lifetimeRange;
	sb::Vector2f _speedRange;
	sb::Vector2f _rotationRange;
	sb::Vector2f _scaleRange;
	Tween _scaleOverLifetime;

	float _emissionInterval;
	float _timeSinceLastEmission;
	std::vector<Particle> _particles;

protected:
	void emitParticle() {
		float timeToLive = sb::random(_lifetimeRange.x, _lifetimeRange.y);
		float speed = sb::random(_speedRange.x, _speedRange.y);
		sb::Vector2f velocity = speed * sb::randomOnCircle(1);
		float scale = sb::random(_scaleRange.x, _scaleRange.y);
		float rotation = sb::random(_rotationRange.x, _rotationRange.y);

		_particles.emplace_back(timeToLive, velocity, sb::Vector2f(scale, scale), rotation);
	}

	void emitParticles(float ds) {
		_timeSinceLastEmission += ds;
		while (_timeSinceLastEmission > _emissionInterval) {
			emitParticle();
			_timeSinceLastEmission -= _emissionInterval;
		}
	}

	static bool isParticleOutdated(const Particle& particle) {
		return particle.lifetime > particle.timeToLive;
	}

	void removeParticles(float ds) {
		_particles.erase(std::remove_if(_particles.begin(), _particles.end(), isParticleOutdated), _particles.end());
	}

	void transformParticles(float ds) {
		for (std::size_t i = 0; i < _particles.size(); i++) {
			_particles[i].quad.translate(ds * _particles[i].velocity);
			float scaleFactor = _scaleOverLifetime.value(_particles[i].lifetime / _particles[i].timeToLive);
			_particles[i].quad.setScale(scaleFactor * _particles[i].startScale);
			_particles[i].quad.setRotation(_particles[i].startRotation);
		}
	}

	void updateParticles(float ds) {
		for (std::size_t i = 0; i < _particles.size(); i++)
			_particles[i].lifetime += ds;
		removeParticles(ds);
		transformParticles(ds);
	}
public:
	ParticleSystem2()
		: _lifetimeRange(2, 2), _rotationRange(0, 2 * sb::Pi), _scaleRange(1, 1),
		_scaleOverLifetime(Tween::None(1)), _emissionInterval(0.1f), _timeSinceLastEmission(0)
	{
	}

	inline void setScaleRange(float min, float max) { _scaleRange = sb::Vector2f(min, max); }

	inline void setSpeedRange(float min, float max) { _speedRange = sb::Vector2f(min, max); }

	inline void setRotationRange(float min, float max) { _rotationRange = sb::Vector2f(min, max); }

	void setScaleOverLifetime(Tween& scaleOverLifetime) { 
		SB_ERROR_IF(scaleOverLifetime.getDuration() > 1, "The duration of the tween must not be larger than 1, i.e. the tween must have normalized time");
		_scaleOverLifetime = scaleOverLifetime; 
	}

	void update(float ds) {
		emitParticles(ds);
		updateParticles(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();

		for (std::size_t i = 0; i < _particles.size(); i++)
			target.draw(_particles[i].quad);
	}
};

void initParticleSystem2(ParticleSystem2& system) {
	system.setRotationRange(0, 360 * sb::ToRadian);
	system.setSpeedRange(0.1f, 0.5f);
	system.setScaleRange(0.05f, 0.1f);
	Tween sizeOverLifetime = Tween().bounceOut(0, 1, 0.2f).quintInOut(1, 0, 0.8f);
	system.setScaleOverLifetime(sizeOverLifetime);
}

void demo2() {
	sb::Window window;
	ParticleSystem2 particleSystem;

	window.getCamera().setWidth(1.5f);
	initParticleSystem2(particleSystem);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		particleSystem.draw(window);
		window.display();
	}
}

class ParticleSystem : public sb::Drawable, public sb::Transformable {
	struct Particle : public sb::Body {
		float lifetime = 0;
		float secondsSinceBirth = 0;
		bool isActive = false;
	};

	sb::Mesh _mesh;
	sb::Texture* _texture;

	std::vector<Particle> _particles;
	std::size_t _numActiveParticles;
	float _secondsSinceLastEmission;
	float _secondsSinceBirth;

	bool _canDie;
	float _lifetime;
	float _emissionRatePerSecond;
	sb::Vector2f _particleLifetimeRange;
	sb::Vector2f _particleSizeRange;
	sb::Vector2f _particleRotationRange;
	sb::Vector2f _particleSpeedRange;
	sb::Vector2f _particleAngularVelocityRange;

protected: 
	static bool isParticleDead(const Particle& particle) {
		return particle.secondsSinceBirth > particle.lifetime;
	}

	void deactivateParticleInMesh(std::size_t meshIndex) {
		_mesh[meshIndex * 6 + 0].position = sb::Vector2f(0, 0);
		_mesh[meshIndex * 6 + 1].position = sb::Vector2f(0, 0);
		_mesh[meshIndex * 6 + 2].position = sb::Vector2f(0, 0);
		_mesh[meshIndex * 6 + 3].position = sb::Vector2f(0, 0);
		_mesh[meshIndex * 6 + 4].position = sb::Vector2f(0, 0);
		_mesh[meshIndex * 6 + 5].position = sb::Vector2f(0, 0);
	}

	void deactivateDeadParticles() {
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive && isParticleDead(_particles[i])) {
				_particles[i].isActive = false;
				deactivateParticleInMesh(i);
				_numActiveParticles--;
			}
		}
	}

	static bool isParticleInactive(const Particle& particle) {
		return !particle.isActive;
	}

	std::size_t findAvailableIndex() {
		std::vector<Particle>::iterator it =
			std::find_if(_particles.begin(), _particles.end(), isParticleInactive);
		return std::distance(_particles.begin(), it);
	}

	void initParticle(Particle& particle) {
		particle.lifetime = sb::random(_particleLifetimeRange.x, _particleLifetimeRange.y);
		particle.velocity = sb::random(_particleSpeedRange.x, _particleSpeedRange.y) * sb::randomOnCircle(1);
		particle.angularVelocity = sb::random(_particleAngularVelocityRange.x, _particleAngularVelocityRange.y);
		float size = sb::random(_particleSizeRange.x, _particleSizeRange.y);
		particle.setScale(size, size);
		particle.setRotation(sb::random(_particleRotationRange.x, _particleRotationRange.y));
		particle.isActive = true;
	}

	void emitParticle() {
		if (_numActiveParticles == _particles.size())
			return;

		std::size_t availableIndex = findAvailableIndex();
		Particle particle;
		initParticle(particle);
		_particles[availableIndex] = particle;
		_numActiveParticles++;
	}

	void emitParticles(float ds) {
		_secondsSinceLastEmission += ds;
		float emissionInterval = 1 / _emissionRatePerSecond;
		while (_secondsSinceLastEmission > emissionInterval) {
			emitParticle();
			_secondsSinceLastEmission -= emissionInterval;
		}
	}

	void updateParticle(Particle& particle, float ds) {
		particle.secondsSinceBirth += ds;
		particle.translate(ds * particle.velocity);
		particle.rotate(ds * particle.angularVelocity);
	}

	void updateParticles(float ds) {
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				updateParticle(_particles[i], ds);
		}
	}

	void updateParticleVertices(Particle& particle, std::size_t index) {
		std::vector<sb::Vector2f> edges(4);
		edges[0] = particle.getTransform() * sb::Vector2f(-0.5f, -0.5f);
		edges[1] = particle.getTransform() * sb::Vector2f(0.5f, -0.5f);
		edges[2] = particle.getTransform() * sb::Vector2f(-0.5f, 0.5f);
		edges[3] = particle.getTransform() * sb::Vector2f(0.5f, 0.5f);

		const sb::Color color(1, 0, 0, 1);
		_mesh[index * 6 + 0] = sb::Vertex(edges[0], sb::Color(1, 0, 0, 1), sb::Vector2f(0, 0));
		_mesh[index * 6 + 1] = sb::Vertex(edges[0], sb::Color(1, 0, 0, 1), sb::Vector2f(0, 0));
		_mesh[index * 6 + 2] = sb::Vertex(edges[1], sb::Color(0, 1, 0, 1), sb::Vector2f(1, 0));
		_mesh[index * 6 + 3] = sb::Vertex(edges[2], sb::Color(0, 0, 1, 1), sb::Vector2f(0, 1));
		_mesh[index * 6 + 4] = sb::Vertex(edges[3], sb::Color(0, 1, 1, 1), sb::Vector2f(1, 1));
		_mesh[index * 6 + 5] = sb::Vertex(edges[3], sb::Color(0, 1, 1, 1), sb::Vector2f(1, 1));
	}

	void updateMesh(float ds) {
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				updateParticleVertices(_particles[i], i);
		}
	}

public:
	ParticleSystem(std::size_t maxNumParticles)
		: _mesh(maxNumParticles * 6, sb::PrimitiveType::TriangleStrip), _texture(NULL),
		_particles(maxNumParticles), _numActiveParticles(0), _secondsSinceLastEmission(0),
		_canDie(false) ,_lifetime(1), _emissionRatePerSecond(1), _particleLifetimeRange(1, 1), 
		_particleSizeRange(0.1f, 0.1f), _particleRotationRange(0, 0), _particleSpeedRange(1, 1)
	{ }

	inline void setEmissionRatePerSecond(float rate) { _emissionRatePerSecond = rate; }

	inline void setParticleLifetimeRange(const sb::Vector2f& lifetimeRange) { _particleLifetimeRange = lifetimeRange; }

	inline void setParticleSizeRange(const sb::Vector2f& sizeRange) { _particleSizeRange = sizeRange; }

	inline void setParticleRotationRange(const sb::Vector2f& rotationRange) { _particleRotationRange = rotationRange; }

	inline void setParticleSpeedRange(const sb::Vector2f& speedRange) { _particleSpeedRange = speedRange; }

	inline void setParticleAngularVelocityRange(const sb::Vector2f& range) { _particleAngularVelocityRange = range; }

	inline void canDie(bool canDie) { _canDie = canDie; }

	void setLifetime(float lifetime) { 
		_canDie = true;
		_lifetime = lifetime; 
	}
	
	bool isAlive() { 
		return !_canDie || _secondsSinceBirth < _lifetime; 
	}

	void update(float ds) { 
		_secondsSinceBirth += ds;

		deactivateDeadParticles();
		if (isAlive()) {	
			emitParticles(ds);
			updateParticles(ds);
			updateMesh(ds);
		}
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) { 
		if (isAlive()) {
			states.transform *= getTransform();
			states.texture = _texture;
			target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
		}
	}
};

void demo3() {
	sb::Window window;
	ParticleSystem particleSystem(100);

	window.getCamera().setWidth(2.5);
	particleSystem.setEmissionRatePerSecond(100);
	particleSystem.setParticleSpeedRange(sb::Vector2f(0.5f, 1));
	particleSystem.setParticleRotationRange(sb::Vector2f(0, 2 * sb::Pi));
	particleSystem.setParticleAngularVelocityRange(sb::Vector2f(-4, 4));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		particleSystem.draw(window);
		window.display();
	}
}


int main() {
	demo3();

	//demo2();

	//demo1();

	//demo0();

	return 0;
}
