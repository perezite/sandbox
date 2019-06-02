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

float computeLinear(float t, float b, float c, float d) {
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

inline float linear(float t, float t0, float t1, float from, float to) {
	float duration = t1 - t0;
	float time = t < t0 ? t0 : t > t1 ? t1 : t;
	return computeLinear(time - t0, from, to - from, duration);
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

	float getDuration() const {
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

	Tween& linear(float from, float to, float duration) {
		addEasing<::linear>(from, to, duration);
		return *this;
	}

	Tween& wait(float value, float duration) {
		addEasing<::linear>(value, value, duration);
		return *this;
	}

	Tween& wait(float duration) {
		float value = !_easingCommands.empty() ? _easingCommands[_easingCommands.size() - 1].to : 0;
		return wait(value, duration);
	}

	static Tween None(float value = 0) {
		return Tween().linear(value, value, 0);
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

sb::Vector2f randomInsideRectangle(float width, float height) {
	float x = sb::random(-width / 2, width / 2);
	float y = sb::random(-height / 2, height / 2);
	return sb::Vector2f(x, y);
}

void sampleRectangle(sb::DrawTarget& target, float width, float height, sb::Mesh& result) {
	for (std::size_t i = 0; i < result.getVertexCount(); i++) {
		const sb::Vector2f position = randomInsideRectangle(width, height);
		result[i] = sb::Vertex(position, sb::Color(1, 0, 0, 1));
	}
}

sb::Vector2f randomInsideDisk(float innerRadius, float outerRadius, float startRadian, float endRadian) {
	float r = sqrtf(sb::random(innerRadius * innerRadius, outerRadius * outerRadius));
	float angle = sb::random(startRadian, endRadian);
	return sb::Vector2f(r * cosf(angle), r * sinf(angle));
}

void sampleDisk(sb::DrawTarget& target, float innerRadius, float outerRadius, float startRadian, float endRadian, sb::Mesh& result) {
	for (std::size_t i = 0; i < result.getVertexCount(); i++) {
		const sb::Vector2f position = randomInsideDisk(innerRadius, outerRadius, startRadian, endRadian);
		result[i] = sb::Vertex(position, sb::Color(1, 0, 0, 1));
	}
}

void demo3() {
	sb::Window window;

	sb::Mesh pointSample(2500, sb::PrimitiveType::Points);
	sampleDisk(window, 0.2f, 0.3f, 45 * sb::ToRadian, 270 * sb::ToRadian, pointSample);


	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(pointSample.getVertices(), pointSample.getPrimitiveType());

		window.display();
	}
}

class Shape {
public:
	virtual ~Shape() {}

	virtual sb::Vector2f random() const = 0;

	virtual float getBoundingRadius() const = 0;

	virtual Shape* clone() const = 0;
};

class Disk : public Shape {
	float _innerRadius;
	float _outerRadius;
	float _startRadian;
	float _endRadian;

public:
	Disk(float outerRadius)
		: _innerRadius(0), _outerRadius(outerRadius), _startRadian(0), _endRadian(2 * sb::Pi)
	{ }

	Disk(float outerRadius, float startRadian, float endRadian)
		: _innerRadius(0), _outerRadius(outerRadius), _startRadian(startRadian), _endRadian(endRadian)
	{ }

	Disk(float innerRadius, float outerRadius, float startRadian, float endRadian)
		: _innerRadius(innerRadius), _outerRadius(outerRadius), _startRadian(startRadian), _endRadian(endRadian)
	{ }

	virtual sb::Vector2f random() const {
		float r = sqrtf(sb::random(_innerRadius * _innerRadius, _outerRadius * _outerRadius));
		float angle = sb::random(_startRadian, _endRadian);
		auto result = sb::Vector2f(r * cosf(angle), r * sinf(angle));
		return result;
	}

	virtual float getBoundingRadius() const { return _outerRadius; }

	virtual Shape* clone() const { 
		return new Disk(*this); 
	}
};

void sample(const Shape& shape, sb::Mesh& result) {
	for (std::size_t i = 0; i < result.getVertexCount(); i++)
		result[i] = sb::Vertex(shape.random(), sb::Color(1, 0, 0, 1));
}

void demo4() {
	sb::Window window;

	sb::Mesh pointSample(2500, sb::PrimitiveType::Points);
	Disk disk(0.1f, 0.3f, 45 * sb::ToRadian, 270 * sb::ToRadian);
	sample(disk, pointSample);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(pointSample.getVertices(), pointSample.getPrimitiveType());

		window.display();
	}
}

void printStats() {
	SB_MESSAGE(sb::Renderer::getNumDrawCalls());
	sb::Renderer::resetStatistics();
}

float lerp(float from, float to, float t) {
	t = t < 0 ? 0 : t > 1 ? 1 : t;
	return (1 - t) * from + t * to;
}

template <class T>
inline void copy(T* destination, const T* source) {
	destination = NULL;
	if (source) 
		destination = new T(*source);
}

template <class T>
inline void copyVector(std::vector<T*>& destination, const std::vector<T*>& source) {
	destination.clear();
	for (std::size_t i = 0; i < source.size(); i++) {
		T* element = NULL;
		copy(element, source[i]);
		destination.push_back(element);
	}

	std::copy(source.begin(), source.end(), destination.begin());
}

class ParticleSystem : public sb::Drawable, public sb::Transformable {
	struct Particle : public sb::Body {
		float lifetime = 0;
		float secondsSinceBirth = 0;
		sb::Vector2f startScale;
		std::vector<sb::Color> startVertexColors = std::vector<sb::Color>(4);
		std::vector<sb::Color> vertexColors = std::vector<sb::Color>(4);
		bool isActive = false;
	};

	struct Burst {
		std::size_t numParticles;
		float emissionTime;
		bool emitted = false;

		Burst(float emissionTime_, std::size_t numParticles_) 
			: numParticles(numParticles_), emissionTime(emissionTime_)
		{ }
	};


public:
	sb::Mesh _mesh;
	sb::Texture* _texture;

	std::vector<Particle> _particles;
	std::vector<Burst> _bursts;
	std::size_t _numActiveParticles;
	float _secondsSinceLastEmission;
	float _secondsSinceBirth;

	bool _canDie;
	float _lifetime;
	float _emissionRatePerSecond;
	float _drag;
	float _angularDrag;
	sb::Vector2f _particleLifetimeRange;
	sb::Vector2f _particleSizeRange;
	sb::Vector2f _particleRotationRange;
	sb::Vector2f _particleSpeedRange;
	sb::Vector2f _particleAngularVelocityRange;
	std::vector<sb::Color> _particleVertexColors;
	std::vector<bool> _hasParticleColorChannelsOverLifetime;
	std::vector<Tween> _particleColorChannelsOverLifetime;
	bool _hasParticleScaleOverLifetime;
	Tween _particleScaleOverLifetime;
	Shape* _emissionShape;
	bool _hasRandomEmissionDirection;

	ParticleSystem* _subSystemOnParticleDeath;
	std::vector<ParticleSystem*> _subSystems;

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

	void spawnSubSystem(const Particle& particle) {
		ParticleSystem* subSystem = new ParticleSystem(*_subSystemOnParticleDeath);
		subSystem->setPosition(particle.getPosition());
		_subSystems.push_back(subSystem);
	}

	void removeDeadParticles() {
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive && isParticleDead(_particles[i])) {
				_particles[i].isActive = false;
				_numActiveParticles--;
				deactivateParticleInMesh(i);
				if (_subSystemOnParticleDeath)
					spawnSubSystem(_particles[i]);
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

	sb::Color random(const sb::Color& left, const sb::Color right) {
		return sb::Color(sb::random(left.r, right.r), sb::random(left.g, right.g),
			sb::random(left.b, right.b), sb::random(left.a, right.a));
	}

	sb::Vector2f getDirection(Particle& particle) {
		bool randomDirection = _hasRandomEmissionDirection || _emissionShape->getBoundingRadius() < 0.0001f;
		return randomDirection ? sb::randomOnCircle(1) : particle.getPosition().normalized();
	}

	void initParticle(Particle& particle) {
		particle.setPosition(_emissionShape->random());
		float size = sb::random(_particleSizeRange.x, _particleSizeRange.y);
		particle.setScale(size, size);
		particle.startScale = sb::Vector2f(size, size);
		particle.setRotation(sb::random(_particleRotationRange.x, _particleRotationRange.y));
		sb::Vector2f direction = getDirection(particle);
		particle.velocity = sb::random(_particleSpeedRange.x, _particleSpeedRange.y) * direction;
		particle.angularVelocity = sb::random(_particleAngularVelocityRange.x, _particleAngularVelocityRange.y);
		particle.lifetime = sb::random(_particleLifetimeRange.x, _particleLifetimeRange.y);
		particle.vertexColors = _particleVertexColors;
		particle.startVertexColors = _particleVertexColors;
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

	void emitBurst(Burst& burst) {
		for (std::size_t i = 0; i < burst.numParticles; i++) 
			emitParticle();

		burst.emitted = true;
	}

	void emitBursts(float ds) {
		for (std::size_t i = 0; i < _bursts.size(); i++) {
			if (!_bursts[i].emitted && _secondsSinceBirth >= _bursts[i].emissionTime)
				emitBurst(_bursts[i]);
		}
	}

	sb::Vector2f computeForce(Particle& particle) {
		sb::Vector2f dragForce = -_drag * particle.velocity;
		return dragForce;
	}

	float computeTorque(Particle& particle) {
		float dragTorque = -_angularDrag * particle.angularVelocity;
		return dragTorque;
	}

	void updateScale(Particle& particle) {
		if (_hasParticleScaleOverLifetime) {
			float t = getNormalizedSecondsSinceBirth(particle);
			particle.setScale(_particleScaleOverLifetime.value(t) * particle.startScale);
		}
	}

	inline float getNormalizedSecondsSinceBirth(const Particle& particle) {
		return particle.secondsSinceBirth / particle.lifetime;
	}

	void updateVertexColorChannel(std::size_t channelIndex, float& colorChannel, const float& startColorChannel, float t) {
		if (_hasParticleColorChannelsOverLifetime[channelIndex])
			colorChannel = startColorChannel * _particleColorChannelsOverLifetime[channelIndex].value(t);
	}

	void updateVertexColor(sb::Color& color, const sb::Color& startColor, Particle& particle) {
		float t = getNormalizedSecondsSinceBirth(particle);

		updateVertexColorChannel(0, color.r, startColor.r, t);
		updateVertexColorChannel(1, color.g, startColor.g, t);		
		updateVertexColorChannel(2, color.b, startColor.b, t);		
		updateVertexColorChannel(3, color.a, startColor.a, t);
	}

	void updateVertexColors(Particle& particle) {
		for (std::size_t i = 0; i < particle.vertexColors.size(); i++)
			updateVertexColor(particle.vertexColors[i], particle.startVertexColors[i], particle);
	}

	void updateParticle(Particle& particle, float ds) {
		particle.secondsSinceBirth += ds;

		particle.velocity += ds * computeForce(particle);
		particle.angularVelocity += ds * computeTorque(particle);

		particle.translate(ds * particle.velocity);
		particle.rotate(ds * particle.angularVelocity);

		updateScale(particle);
		updateVertexColors(particle);
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
		_mesh[index * 6 + 0] = sb::Vertex(edges[0], particle.vertexColors[0], sb::Vector2f(0, 0));
		_mesh[index * 6 + 1] = sb::Vertex(edges[0], particle.vertexColors[0], sb::Vector2f(0, 0));
		_mesh[index * 6 + 2] = sb::Vertex(edges[1], particle.vertexColors[1], sb::Vector2f(1, 0));
		_mesh[index * 6 + 3] = sb::Vertex(edges[2], particle.vertexColors[2], sb::Vector2f(0, 1));
		_mesh[index * 6 + 4] = sb::Vertex(edges[3], particle.vertexColors[3], sb::Vector2f(1, 1));
		_mesh[index * 6 + 5] = sb::Vertex(edges[3], particle.vertexColors[3], sb::Vector2f(1, 1));
	}

	void updateMesh(float ds) {
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				updateParticleVertices(_particles[i], i);
		}
	}

	inline static bool isParticleSystemDead(ParticleSystem* particleSystem) { return !particleSystem->isAlive(); }

	inline static void deleteIfDead(ParticleSystem* particleSystem) {
		if (isParticleSystemDead(particleSystem))
			delete particleSystem;
	}

	inline void deleteDeadSubSystems(float ds) {
		std::for_each(_subSystems.begin(), _subSystems.end(), deleteIfDead);
		_subSystems.erase(std::remove_if(_subSystems.begin(), _subSystems.end(), isParticleSystemDead), _subSystems.end());
	}

	void updateSubSystems(float ds);

public:
	ParticleSystem(std::size_t maxNumParticles)
		: _mesh(maxNumParticles * 6, sb::PrimitiveType::TriangleStrip), _texture(NULL),
		_particles(maxNumParticles), _numActiveParticles(0),
		_secondsSinceLastEmission(0), _secondsSinceBirth(0),
		_canDie(false) ,_lifetime(1), _emissionRatePerSecond(1), _drag(0), _angularDrag(0),
		_particleLifetimeRange(1, 1), _particleSizeRange(0.1f, 0.1f), _particleRotationRange(0, 0), 
		_particleSpeedRange(1, 1),_particleVertexColors(4, sb::Color(1, 0, 0, 1)), _hasParticleColorChannelsOverLifetime(4, false),
		_particleColorChannelsOverLifetime(4), _hasParticleScaleOverLifetime(false), _emissionShape(new Disk(0)), 
		_hasRandomEmissionDirection(false), _subSystemOnParticleDeath(NULL)
	{ }

	ParticleSystem(const ParticleSystem& other) {
		*this = other;
		this->_emissionShape = other._emissionShape->clone();
		copy(this->_subSystemOnParticleDeath, other._subSystemOnParticleDeath);
		copyVector(this->_subSystems, other._subSystems);
	}

	virtual ~ParticleSystem() {
		for (std::size_t i = 0; i < _subSystems.size(); i++)
			delete _subSystems[i];
		if (_subSystemOnParticleDeath)
			delete _subSystemOnParticleDeath;
		delete _emissionShape;
	}

	inline void setEmissionRatePerSecond(float rate) { _emissionRatePerSecond = rate; }

	inline void setDrag(float drag) { _drag = drag; }

	inline void setAngularDrag(float angularDrag) { _angularDrag = angularDrag; }

	inline void setParticleLifetimeRange(const sb::Vector2f& lifetimeRange) { _particleLifetimeRange = lifetimeRange; }

	inline void setParticleSizeRange(const sb::Vector2f& sizeRange) { _particleSizeRange = sizeRange; }

	inline void setParticleRotationRange(const sb::Vector2f& rotationRange) { _particleRotationRange = rotationRange; }

	inline void setParticleSpeedRange(const sb::Vector2f& speedRange) { _particleSpeedRange = speedRange; }

	inline void setParticleAngularVelocityRange(const sb::Vector2f& range) { _particleAngularVelocityRange = range; }

	inline void canDie(bool canDie) { _canDie = canDie; }

	inline void hasRandomEmissionDirection(bool hasRandomEmission) { _hasRandomEmissionDirection = hasRandomEmission; }

	void addBurst(float emissionTime, std::size_t _numParticles) {
		_bursts.emplace_back(emissionTime, _numParticles);
	}
	void setParticleVertexColor(std::size_t index, const sb::Color& color) {
		SB_ERROR_IF(index > 4, "Vertex index out of range");
		_particleVertexColors[index] = color;
	}

	void setParticleColor(const sb::Color& color) { 
		std::fill(_particleVertexColors.begin(), _particleVertexColors.end(), color); 
	}

	void setParticleColorChannelOverLifetime(std::size_t channelIndex, const Tween& particleColorChannelOverLifetime) {
		SB_ERROR_IF(channelIndex > 4, "Color channel index out of range");
		SB_ERROR_IF(particleColorChannelOverLifetime.getDuration() > 1, "Tween duration out of range");
		_particleColorChannelsOverLifetime[channelIndex] = particleColorChannelOverLifetime;
		_hasParticleColorChannelsOverLifetime[channelIndex] = true;
	}

	void setParticleScaleOverLifetime(const Tween& particleScaleOverLifetime) {
		SB_ERROR_IF(particleScaleOverLifetime.getDuration() > 1, "Tween duration out of range");
		_particleScaleOverLifetime = particleScaleOverLifetime;
		_hasParticleScaleOverLifetime = true;
	}

	template <class T>
	void setEmissionShape(const T& shape) {
		delete _emissionShape;
		_emissionShape = new T(shape);
	}

	void setLifetime(float lifetime) { 
		_canDie = true;
		_lifetime = lifetime; 
	}

	void setSubSystemOnParticleDeath(const ParticleSystem& subSystem) {
		if (_subSystemOnParticleDeath)
			delete _subSystemOnParticleDeath;
		_subSystemOnParticleDeath = new ParticleSystem(subSystem);
	}
	
	bool isAlive() { 
		return !_canDie || _secondsSinceBirth < _lifetime || !_subSystems.empty(); 
	}

	void update(float ds) { 
		_secondsSinceBirth += ds;

		removeDeadParticles();
		if (isAlive()) {	
			emitParticles(ds);
			emitBursts(ds);
			updateParticles(ds);
			updateMesh(ds);
		}

		deleteDeadSubSystems(ds);
		updateSubSystems(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) { 
		if (isAlive()) {
			states.transform *= getTransform();
			states.texture = _texture;
			target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
		}
	}
};

void ParticleSystem::updateSubSystems(float ds) {
	for (std::size_t i = 0; i < _subSystems.size(); i++)
		_subSystems[i]->update(ds);
}

void init6(ParticleSystem& system);

void demo6() {
	sb::Window window;
	ParticleSystem particleSystem(1);
	particleSystem.setEmissionRatePerSecond(1000);
	ParticleSystem particleSystem2(particleSystem);

	window.getCamera().setWidth(2.5);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem2.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		particleSystem2.draw(window);
		window.display();
	}
}

void init6(ParticleSystem& system) {
}

void setParticleColor(ParticleSystem& system, float alpha = 1) {
	system.setParticleVertexColor(0, sb::Color(1, 0, 0, alpha));
	system.setParticleVertexColor(1, sb::Color(0, 1, 0, alpha));
	system.setParticleVertexColor(2, sb::Color(0, 0, 1, alpha));
	system.setParticleVertexColor(3, sb::Color(0, 1, 1, alpha));
}

void init6b(ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleLifetimeRange(sb::Vector2f(1, 1));
	system.setParticleSpeedRange(sb::Vector2f(0, 0.05f));
	system.setParticleSizeRange(sb::Vector2f(0.01f, 0.02f));

	system.setParticleRotationRange(sb::Vector2f(0, 2 * sb::Pi));
	system.setParticleAngularVelocityRange(sb::Vector2f(-4, 4));
	system.setEmissionShape(Disk(0.2f, 0.3f, 45 * sb::ToRadian, 225 * sb::ToRadian));
	system.hasRandomEmissionDirection(false);
	system.addBurst(1, 300);
	system.addBurst(3, 600);

	setParticleColor(system);
	system.setScale(2);
}

void init6c(ParticleSystem& system) {
	system.setEmissionRatePerSecond(10);
	system.setParticleLifetimeRange(sb::Vector2f(3, 3));
	system.setParticleSpeedRange(sb::Vector2f(0.2f, 0.2f));
	system.setParticleAngularVelocityRange(sb::Vector2f(4, 4));
	system.setDrag(2);
	system.setAngularDrag(2);

	setParticleColor(system);
	system.setScale(0.5f);
}


void init6d(ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleSizeRange(sb::Vector2f(0.1f, 0.2f));
	system.setParticleColorChannelOverLifetime(3, Tween().quintInOut(1, 0, 1));

	setParticleColor(system, 0.5f);
	system.setScale(0.5f);
}

void init6e(ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleSpeedRange(sb::Vector2f(0.5f, 1));
	system.setParticleAngularVelocityRange(sb::Vector2f(0, 2 * sb::Pi));
	system.setAngularDrag(0.5f);
	system.setParticleSizeRange(sb::Vector2f(0.05f, 0.15f));
	system.setParticleLifetimeRange(sb::Vector2f(1, 2));
	system.setParticleScaleOverLifetime(Tween().quintInOut(0, 1, 0.1f).quintInOut(1, 0, 0.4f));
	system.setParticleColorChannelOverLifetime(3, Tween().wait(1, 0.1f).quintInOut(1, 0, 0.4f));
	system.setEmissionShape(Disk(0.1f, 0.6f, 235 * sb::ToRadian, 305 * sb::ToRadian));

	setParticleColor(system);
	system.setScale(1);
}

void init5(ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleLifetimeRange(sb::Vector2f(1, 1));
	system.setParticleSpeedRange(sb::Vector2f(0, 1));
	system.setParticleSizeRange(sb::Vector2f(0.01f, 0.02f));

	system.setParticleRotationRange(sb::Vector2f(0, 2 * sb::Pi));
	system.setParticleAngularVelocityRange(sb::Vector2f(-4, 4));
	system.setEmissionShape(Disk(0.2f, 0.3f, 0, 360));
	system.hasRandomEmissionDirection(true);
	system.addBurst(1, 300);
	system.addBurst(3, 600);

	system.setParticleVertexColor(0, sb::Color(1, 0, 0, 0.9f));
	system.setParticleVertexColor(1, sb::Color(0, 1, 0, 0.9f));
	system.setParticleVertexColor(2, sb::Color(0, 0, 1, 0.9f));
	system.setParticleVertexColor(3, sb::Color(0, 1, 1, 0));
}

void demo5() {
	sb::Window window;
	ParticleSystem particleSystem(1000);

	window.getCamera().setWidth(2.5);
	init5(particleSystem);
	particleSystem.setScale(2);

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
	demo6();

	//demo5();

	//demo4();

	//demo3();

	//demo2();

	//demo1();

	//demo0();

	return 0;
}
