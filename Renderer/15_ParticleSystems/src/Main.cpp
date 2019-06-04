#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "Triangle.h"
#include "Stopwatch.h"
#include "Math.h"
#include "Body.h"
#include "Tween.h"
#include "Easing.h"
#include "Disk.h"
#include "ParticleSystem.h"
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
			position = sb::Easing::bounceOut(t, 1, 2, -0.4f, 0.4f);
		else
			position = sb::Easing::quintInOut(t, 2.5f, 4.5f, 0.4f, -0.5f);
		quad.setPosition(position, position);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

void demo1() {
	sb::Window window;
	sb::Quad quad;

	window.getCamera().setWidth(1.5f);
	sb::Tween tween = sb::Tween().bounceOut(-0.4f, 0.4f, 2).quintInOut(0.4f, -0.5f, 3);
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
	sb::Tween _scaleOverLifetime;

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
		_scaleOverLifetime(sb::Tween::None(1)), _emissionInterval(0.1f), _timeSinceLastEmission(0)
	{
	}

	inline void setScaleRange(float min, float max) { _scaleRange = sb::Vector2f(min, max); }

	inline void setSpeedRange(float min, float max) { _speedRange = sb::Vector2f(min, max); }

	inline void setRotationRange(float min, float max) { _rotationRange = sb::Vector2f(min, max); }

	void setScaleOverLifetime(sb::Tween& scaleOverLifetime) { 
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
	sb::Tween sizeOverLifetime = sb::Tween().bounceOut(0, 1, 0.2f).quintInOut(1, 0, 0.8f);
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

void sample(const sb::Shape& shape, sb::Mesh& result) {
	for (std::size_t i = 0; i < result.getVertexCount(); i++)
		result[i] = sb::Vertex(shape.random(), sb::Color(1, 0, 0, 1));
}

void demo4() {
	sb::Window window;

	sb::Mesh pointSample(2500, sb::PrimitiveType::Points);
	sb::Disk disk(0.1f, 0.3f, 45 * sb::ToRadian, 270 * sb::ToRadian);
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

void setParticleRainbowColor(sb::ParticleSystem& system, float alpha = 1) {
	system.setParticleVertexColor(0, sb::Color(1, 0, 0, alpha));
	system.setParticleVertexColor(1, sb::Color(0, 1, 0, alpha));
	system.setParticleVertexColor(2, sb::Color(0, 0, 1, alpha));
	system.setParticleVertexColor(3, sb::Color(0, 1, 1, alpha));
}

void init5(sb::ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleLifetimeRange(sb::Vector2f(1, 1));
	system.setParticleSpeedRange(sb::Vector2f(0, 1));
	system.setParticleSizeRange(sb::Vector2f(0.01f, 0.02f));

	system.setParticleRotationRange(sb::Vector2f(0, 2 * sb::Pi));
	system.setParticleAngularVelocityRange(sb::Vector2f(-4, 4));
	system.setEmissionShape(sb::Disk(0.2f, 0.3f, 0, 360));
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
	sb::ParticleSystem particleSystem(1000);

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

void init6b(sb::ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleLifetimeRange(sb::Vector2f(1, 1));
	system.setParticleSpeedRange(sb::Vector2f(0, 0.05f));
	system.setParticleSizeRange(sb::Vector2f(0.01f, 0.02f));

	system.setParticleRotationRange(sb::Vector2f(0, 2 * sb::Pi));
	system.setParticleAngularVelocityRange(sb::Vector2f(-4, 4));
	system.setEmissionShape(sb::Disk(0.2f, 0.3f, 45 * sb::ToRadian, 225 * sb::ToRadian));
	system.hasRandomEmissionDirection(false);
	system.addBurst(1, 300);
	system.addBurst(3, 600);

	setParticleRainbowColor(system);
	system.setScale(2);
}

void init6c(sb::ParticleSystem& system) {
	system.setEmissionRatePerSecond(10);
	system.setParticleLifetimeRange(sb::Vector2f(3, 3));
	system.setParticleSpeedRange(sb::Vector2f(0.2f, 0.2f));
	system.setParticleAngularVelocityRange(sb::Vector2f(4, 4));
	system.setParticleDrag(2);
	system.setAngularParticleDrag(2);

	setParticleRainbowColor(system);
	system.setScale(0.5f);
}

void init6d(sb::ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleSizeRange(sb::Vector2f(0.1f, 0.2f));
	system.setParticleColorChannelOverLifetime(3, sb::Tween().quintInOut(1, 0, 1));

	setParticleRainbowColor(system, 0.5f);
	system.setScale(0.5f);
}

void init6e(sb::ParticleSystem& system) {
	system.setEmissionRatePerSecond(100);
	system.setParticleSpeedRange(sb::Vector2f(0.5f, 1));
	system.setParticleAngularVelocityRange(sb::Vector2f(0, 2 * sb::Pi));
	system.setAngularParticleDrag(0.5f);
	system.setParticleSizeRange(sb::Vector2f(0.05f, 0.15f));
	system.setParticleLifetimeRange(sb::Vector2f(1, 2));
	system.setParticleScaleOverLifetime(sb::Tween().quintInOut(0, 1, 0.1f).quintInOut(1, 0, 0.4f));
	system.setParticleColorChannelOverLifetime(3, sb::Tween().wait(1, 0.1f).quintInOut(1, 0, 0.4f));
	system.setEmissionShape(sb::Disk(0.1f, 0.6f, 235 * sb::ToRadian, 305 * sb::ToRadian));

	setParticleRainbowColor(system);
	system.setScale(1);
}

void init6(sb::ParticleSystem& system, sb::ParticleSystem& subSystem) {
	subSystem.setParticleSizeRange(sb::Vector2f(0.15f, 0.15f));
	subSystem.setEmissionRatePerSecond(0);
	subSystem.addBurst(0, 50);
	subSystem.setEmissionShape(sb::Disk(1));
	subSystem.setDrag(0.005f);
	subSystem.setLifetime(1);

	system.setEmissionRatePerSecond(2);
	system.setSubSystemOnParticleDeath(subSystem);

	setParticleRainbowColor(system);
}

void demo6() {
	sb::Window window;
	sb::ParticleSystem particleSystem(1000);
	particleSystem.id = "main";
	sb::ParticleSystem subSystem(100);

	init6(particleSystem, subSystem);
	window.getCamera().setWidth(2.5);

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

class TweenVisualization : public sb::Drawable {
	struct NamedTween {
		std::string name;
		sb::Tween tween;
	};

	sb::Quad _scalingQuad;
	sb::Quad _movingQuad;
	sb::Triangle _fadingTriangle;
	sb::Mesh _curve;
	std::vector<NamedTween> _namedTweens;
	std::size_t _tweenIndex;
	float _elapsedSeconds;

protected:

public:
	TweenVisualization()
		: _curve(1000, sb::PrimitiveType::TriangleStrip), _tweenIndex(-1), _elapsedSeconds(0)
	{
		initializeTweens();
		nextTween();
		_scalingQuad.setScale(0.1f, 0.1f);
		_scalingQuad.setPosition(0, 0.4f);
		_movingQuad.setScale(0.1f, 0.1f);
		_movingQuad.setPosition(-0.4f, 0.4f);
		_fadingTriangle.setScale(0.1f, 0.1f);
		_fadingTriangle.setPosition(0, -0.4f);
	}

	void initializeTweens() {
		_namedTweens = {
			{"bounceInOut", sb::Tween().bounceInOut(0, 1, 1) },
			{"bounceOut", sb::Tween().bounceOut(0, 1, 1)},
			{"bounceIn", sb::Tween().bounceIn(0, 1, 1) },
			{"elasticInOut", sb::Tween().elasticInOut(0, 1, 1) },
			{"elasticOut", sb::Tween().elasticOut(0, 1, 1) },
			{"elasticIn", sb::Tween().elasticIn(0, 1, 1) },
			{"backInOut", sb::Tween().backInOut(0, 1, 1) },
			{"backOut", sb::Tween().backOut(0, 1, 1) },
			{"backIn", sb::Tween().backIn(0, 1, 1) },
			{"circInOut", sb::Tween().circInOut(0, 1, 1) },
			{"circOut", sb::Tween().circOut(0, 1, 1) },
			{"circIn", sb::Tween().circIn(0, 1, 1) },
			{"expoInOut", sb::Tween().expoInOut(0, 1, 1) },
			{"expoOut", sb::Tween().expoOut(0, 1, 1) },
			{"expoIn", sb::Tween().expoIn(0, 1, 1) },
			{"quintInOut", sb::Tween().quintInOut(0, 1, 1)},
			{"quintOut", sb::Tween().quintOut(0, 1, 1) },
			{"quintIn", sb::Tween().quintIn(0, 1, 1) },
			{"quartInOut", sb::Tween().quartInOut(0, 1, 1)},
			{"quartOut", sb::Tween().quartOut(0, 1, 1)},
			{"quartIn", sb::Tween().quartIn(0, 1, 1)},
			{"cubicInOut", sb::Tween().cubicInOut(0, 1, 1)},
			{"cubicOut", sb::Tween().cubicOut(0, 1, 1)},
			{"cubicIn", sb::Tween().cubicIn(0, 1, 1)},
			{"quadInOut", sb::Tween().quadInOut(0, 1, 1)},
			{"quadOut", sb::Tween().quadOut(0, 1, 1)},
			{"quadIn", sb::Tween().quadIn(0, 1, 1)},
			{"sineInOut", sb::Tween().sineInOut(0, 1, 1)},
			{"sineOut", sb::Tween().sineOut(0, 1, 1)},
			{"sineIn", sb::Tween().sineIn(0, 1, 1)},
			{"linear", sb::Tween().linear(0, 1, 1)}
		};
	}

	inline const sb::Tween& getCurrentTween() const { return _namedTweens[_tweenIndex].tween; }

	void getCurveValues(std::vector<float>& values) {
		float delta = 1 / float(values.size() - 1);
		for (std::size_t i = 0; i < values.size(); i++)
			values[i] = _namedTweens[_tweenIndex].tween.value(i * delta);
	}

	void computeCurve() {
		static const std::size_t numPoints = _curve.getVertexCount() / 2;
		float delta = 1 / float(numPoints - 1);
		static const float thickness = 0.01f;
		std::vector<float> values(numPoints);
		getCurveValues(values);
		for (std::size_t i = 0; i < numPoints; i++) {
			sb::Vector2f bottom(-0.5f + i * delta, -0.5f + values[i] - thickness / 2);
			sb::Vector2f top(-0.5f + i * delta, -0.5f + values[i] + thickness / 2);
			_curve[i * 2 + 0] = sb::Vertex(bottom, sb::Color(1, 0, 0, 1));
			_curve[i * 2 + 1] = sb::Vertex(top, sb::Color(1, 0, 0, 1));
		}
	}

	void nextTween() {
		_tweenIndex = (_tweenIndex + 1) % _namedTweens.size();
		SB_MESSAGE(_namedTweens[_tweenIndex].name);
		computeCurve();
	}

	void scale(float ds) {
		float t = sb::oscillate(_elapsedSeconds, 1);
		float value = getCurrentTween().value(t);
		_scalingQuad.setScale(value * sb::Vector2f(0.1f, 0.1f));
	}

	void move(float ds) {
		float t = sb::oscillate(_elapsedSeconds, 1);
		float value = getCurrentTween().value(t);
		sb::Vector2f position = 
			sb::lerp(value, sb::Vector2f(-0.4f, 0.4f), sb::Vector2f(0.4f, -0.4f));
		_movingQuad.setPosition(position);
	}

	void fade(float ds) {
		float t = sb::oscillate(_elapsedSeconds, 1);
		float value = getCurrentTween().value(t);
		for (std::size_t i = 0; i < _fadingTriangle.getMesh().getVertexCount(); i++)
			_fadingTriangle.getMesh()[i].color.a = value;
	}

	void update(float ds) {
		_elapsedSeconds += ds;
		scale(ds);
		move(ds);
		fade(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		target.draw(_curve.getVertices(), _curve.getPrimitiveType(), states);
		target.draw(_scalingQuad, states);
		target.draw(_movingQuad, states);
		target.draw(_fadingTriangle, states);
	}

};

void demo7() {
	sb::Window window;
	TweenVisualization visualization;

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		visualization.update(ds);
		if (sb::Input::isTouchGoingDown(1))
			visualization.nextTween();

		window.clear(sb::Color(1, 1, 1, 1));
		visualization.draw(window);
		window.display();
	}
}

void init8(sb::ParticleSystem& system) {
	setParticleRainbowColor(system);
	system.setEmissionRatePerSecond(100);

	system.setScale(0.1f, 0.1f);
}

void demo8() {
	sb::Window window;
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1000);

	init8(particleSystem);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);
		if (sb::Input::isTouchGoingDown(1))
			particleSystem.reset();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);
		window.display();
	}
}

void init10(sb::ParticleSystem& system, sb::ParticleSystem& subSystem, sb::Texture& texture) {
	system.setParticleLifetimeRange(sb::Vector2f(1, 1));
	system.setParticleSpeedRange(sb::Vector2f(1, 1));
	system.setParticleSizeRange(0.8f * sb::Vector2f(0.175f, 0.65f));
	system.setParticleColor(sb::Color(1, 1, 1, 0.3f));
	system.setEmissionRatePerSecond(100);
	system.setParticleDrag(0.1f);
	system.setParticleColorChannelOverLifetime(3, sb::Tween().linear(1, 0, 1));
	system.setParticleScaleOverLifetime(sb::Tween().bounceOut(0, 1, 0.1f).quadInOut(1, 0, 0.9f));
	system.setEmissionShape(sb::Disk(0, 0.6f, (270 - 28) * sb::ToRadian, (270 + 28) * sb::ToRadian));
	system.setTexture(texture);

	subSystem.setLifetime(1);
	subSystem.canDie(true);
	subSystem.setParticleLifetimeRange(sb::Vector2f(0.5f, 0.5f));
	subSystem.setParticleSpeedRange(sb::Vector2f(0.5f, 0.5f));
	subSystem.setParticleSizeRange(1.0f * sb::Vector2f(0.01f, 0.1f));
	subSystem.setParticleColor(sb::Color(1, 1, 1, 0.3f));
	subSystem.hasRandomEmissionDirection(true);
	subSystem.setEmissionRatePerSecond(0);
	subSystem.addBurst(0, 1);
	subSystem.setParticleDrag(50);
	subSystem.setParticleColorChannelOverLifetime(3, sb::Tween().linear(1, 0, 1));
	subSystem.setTexture(texture);

	system.setSubSystemOnParticleDeath(subSystem);
	system.setScale(0.25f);
}

void demo10() {
	sb::Window window;
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1000);
	sb::ParticleSystem particleSubSystem(10);

	texture.loadFromAsset("Textures/GreenPropulsion.png");
	init10(particleSystem, particleSubSystem, texture);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);
		window.display();
	}
}

void init9(sb::ParticleSystem& particleSystem, sb::ParticleSystem& subParticleSystem) {
	subParticleSystem.setParticleColor(sb::Color(1, 0, 0, 1));
	subParticleSystem.setLifetime(1);
	subParticleSystem.setEmissionRatePerSecond(0);
	subParticleSystem.addBurst(0, 2);
	subParticleSystem.hasRandomEmissionDirection(true);
	subParticleSystem.id = "sub";

	setParticleRainbowColor(particleSystem);
	particleSystem.setScale(0.1f, 0.1f);
	particleSystem.setSubSystemOnParticleDeath(subParticleSystem);
	particleSystem.id = "main";
}

void input9(sb::Window& window, sb::ParticleSystem& system) {
	if (sb::Input::isTouchGoingDown(1)) {
		const sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (touch.x <= 0)
			system.setEmissionRatePerSecond(sb::clamp(system.getEmissionRatePerSecond() - 5, 0, 1000));
		else
			system.setEmissionRatePerSecond(system.getEmissionRatePerSecond() + 5);

		SB_MESSAGE("Emsision rate:" << system.getEmissionRatePerSecond());
	}
}

void demo9() {
	sb::Window window;
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1000);
	sb::ParticleSystem subParticleSystem(2);

	init9(particleSystem, subParticleSystem);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);
		input9(window, particleSystem);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);
		window.display();
	}
}

int main() {
	demo10();

	//demo9();

	//demo8();

	//demo7();

	//demo6();

	//demo5();

	//demo4();

	//demo3();

	//demo2();

	//demo1();

	//demo0();

	return 0;
}
