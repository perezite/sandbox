#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include "Triangle.h"
#include "Texture.h"
#include "Sprite.h"
#include "Math.h"
#include "Stopwatch.h"

class ParticleSystem : public sb::Drawable {
	const sb::Vector2f m_scaleRange;
	std::size_t m_numParticles;
	sb::Mesh m_mesh;
	std::vector<sb::Vector2f> m_velocities;
	std::size_t m_updateIndex;
	bool m_isGrowing;
	float m_elapsed;

protected:
	void setParticle(sb::Transform& transform, std::size_t index) {
		std::vector<sb::Vector2f> edges(4);
		edges[0] = transform * sb::Vector2f(-0.5f, -0.5f);
		edges[1] = transform * sb::Vector2f( 0.5f, -0.5f);
		edges[2] = transform * sb::Vector2f(-0.5f,  0.5f);
		edges[3] = transform * sb::Vector2f( 0.5f,  0.5f);

		m_mesh[index * 6 + 0] = sb::Vertex(edges[0], sb::Color(1, 0, 0, 1), sb::Vector2f(0, 0));
		m_mesh[index * 6 + 1] = sb::Vertex(edges[0], sb::Color(1, 0, 0, 1), sb::Vector2f(0, 0));
		m_mesh[index * 6 + 2] = sb::Vertex(edges[1], sb::Color(0, 1, 0, 1), sb::Vector2f(1, 0));
		m_mesh[index * 6 + 3] = sb::Vertex(edges[2], sb::Color(0, 0, 1, 1), sb::Vector2f(0, 1));
		m_mesh[index * 6 + 4] = sb::Vertex(edges[3], sb::Color(0, 1, 1, 1), sb::Vector2f(1, 1));
		m_mesh[index * 6 + 5] = sb::Vertex(edges[3], sb::Color(0, 1, 1, 1), sb::Vector2f(1, 1));
	}

	void randomizeParticle(std::size_t index) {
		sb::Vector2f position = sb::random2D(-0.1f, 0.1f);
		sb::Vector2f scale = sb::random(m_scaleRange.x, m_scaleRange.y);
		float rotation = sb::random(0, 2 * sb::Pi);
		sb::Transform transform(position, scale, rotation);
		setParticle(transform, index);
		m_velocities[index] = sb::random(0.05f, 0.5f) * position.normalized();
	}

	void hideParticle(std::size_t index) {
		m_mesh[index * 6 + 0].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 1].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 2].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 3].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 4].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 5].position = sb::Vector2f(0, 0);
	}

	void scaleParticle(std::size_t index) {
		if (m_isGrowing)
			randomizeParticle(index);
		else
			hideParticle(index);
	}

	void scaleParticle() {
		scaleParticle(m_updateIndex);
		m_updateIndex++;
		if (m_updateIndex == m_numParticles - 1) {
			m_isGrowing = !m_isGrowing;
			m_updateIndex = 0;
		}
	}

	void scale(float ds) {
		m_elapsed += ds;
		float decayInterval = 0.0005f;
		while (m_elapsed > decayInterval) {
			scaleParticle();
			m_elapsed -= decayInterval;
		}
	}

	void moveParticle(float ds, std::size_t index) {
		m_mesh[index * 6 + 0].position += ds * m_velocities[index];
		m_mesh[index * 6 + 1].position += ds * m_velocities[index];
		m_mesh[index * 6 + 2].position += ds * m_velocities[index];
		m_mesh[index * 6 + 3].position += ds * m_velocities[index];
		m_mesh[index * 6 + 4].position += ds * m_velocities[index];
		m_mesh[index * 6 + 5].position += ds * m_velocities[index];
	}

	void move(float ds) {
		for (std::size_t i = 0; i < m_numParticles; i++)
			moveParticle(ds, i);
	}


public:
	ParticleSystem(std::size_t numParticles)
		: m_scaleRange(0.02f, 0.04f), m_numParticles(numParticles), 
			m_mesh(m_numParticles * 6, sb::PrimitiveType::TriangleStrip), 
			m_velocities(numParticles), m_updateIndex(0),
			m_isGrowing(true), m_elapsed(0)
	{ }

	void update(float ds) {
		scale(ds);
		move(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), drawStates);
	}
};

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	static sb::Stopwatch sw;
	float elapsed = sw.getElapsedSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void demo1() {
	sb::Window window;
	ParticleSystem particles(5000);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		particles.update(getDeltaSeconds());

		window.clear();
		window.draw(particles);
		window.display();
	}
}

void printStats(std::size_t frequency = 50) {
	static sb::Stopwatch sw;
	static std::size_t numFrames = 0;
	if (numFrames % frequency == 0) {
		float fps = (float)numFrames / sw.getElapsedSeconds();
		std::size_t numDrawCalls = sb::Renderer::getNumDrawCalls();
		SDL_Log("FPS: %f, Draw Calls: %d", fps, numDrawCalls);
		sw.reset();
		numFrames = 0;
	}
	sb::Renderer::resetStatistics();
	numFrames++;
}

class ParticleSystem2 : public sb::Drawable {
	const sb::Vector2f m_scaleRange;
	std::size_t m_numParticles;
	sb::Mesh m_mesh;
	std::vector<sb::Vector2f> m_velocities;
	const sb::Texture* m_texture;

protected:
	void setParticle(sb::Transform& transform, std::size_t index) {
		std::vector<sb::Vector2f> edges(4);
		edges[0] = transform * sb::Vector2f(-0.5f, -0.5f);
		edges[1] = transform * sb::Vector2f(0.5f, -0.5f);
		edges[2] = transform * sb::Vector2f(-0.5f, 0.5f);
		edges[3] = transform * sb::Vector2f(0.5f, 0.5f);

		const sb::Color color(1, 1, 1, 0.3f);
		m_mesh[index * 6 + 0] = sb::Vertex(edges[0], color, sb::Vector2f(0, 0));
		m_mesh[index * 6 + 1] = sb::Vertex(edges[0], color, sb::Vector2f(0, 0));
		m_mesh[index * 6 + 2] = sb::Vertex(edges[1], color, sb::Vector2f(1, 0));
		m_mesh[index * 6 + 3] = sb::Vertex(edges[2], color, sb::Vector2f(0, 1));
		m_mesh[index * 6 + 4] = sb::Vertex(edges[3], color, sb::Vector2f(1, 1));
		m_mesh[index * 6 + 5] = sb::Vertex(edges[3], color, sb::Vector2f(1, 1));
	}

	void randomizeParticle(std::size_t index) {
		sb::Vector2f position = sb::random2D(-1, 1);
		sb::Vector2f scale = sb::random(m_scaleRange.x, m_scaleRange.y);
		float rotation = sb::random(0, 2 * sb::Pi);
		sb::Transform transform(position, scale, rotation);
		setParticle(transform, index);
		m_velocities[index] = sb::random(0.05f, 0.5f) * position.normalized();
	}

	void init() {
		for (std::size_t i = 0; i < m_numParticles; i++)
			randomizeParticle(i);
	}

	void moveParticle(float ds, std::size_t index) {
		m_mesh[index * 6 + 0].position += ds * m_velocities[index];
		m_mesh[index * 6 + 1].position += ds * m_velocities[index];
		m_mesh[index * 6 + 2].position += ds * m_velocities[index];
		m_mesh[index * 6 + 3].position += ds * m_velocities[index];
		m_mesh[index * 6 + 4].position += ds * m_velocities[index];
		m_mesh[index * 6 + 5].position += ds * m_velocities[index];
	}

	void move(float ds) {
		for (std::size_t i = 0; i < m_numParticles; i++)
			moveParticle(ds, i);
	}

public:
	ParticleSystem2(std::size_t numParticles)
		: m_scaleRange(0, 0.1f), m_numParticles(numParticles),
		m_mesh(m_numParticles * 6, sb::PrimitiveType::TriangleStrip),
		m_velocities(numParticles), m_texture(NULL)
	{ 
		init();
	}

	void update(float ds) {
		move(ds);
	}

	inline void setTexture(const sb::Texture& texture) { m_texture = &texture; }

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();
		drawStates.texture = m_texture;
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), drawStates);
	}
};

void demo2() {
	sb::Window window;
	ParticleSystem2 particles(5000);
	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	particles.setTexture(greenPropulsion);
	particles.setScale(0.2f, 0.2f);

	while (window.isOpen()) {
		// float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		// particles.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particles);
		window.display();
		printStats();
	}
}

class ParticleSystem3 : public sb::Drawable {
	sb::Vector2f m_particleSizeRange;
	std::size_t m_numParticles;
	sb::Mesh m_mesh;
	std::vector<sb::Vector2f> m_velocities;
	std::size_t m_updateIndex;
	bool m_isGrowing;
	float m_elapsed;
	const sb::Texture* m_texture;

protected:
	void setParticle(sb::Transform& transform, std::size_t index) {
		std::vector<sb::Vector2f> edges(4);
		edges[0] = transform * sb::Vector2f(-0.5f, -0.5f);
		edges[1] = transform * sb::Vector2f(0.5f, -0.5f);
		edges[2] = transform * sb::Vector2f(-0.5f, 0.5f);
		edges[3] = transform * sb::Vector2f(0.5f, 0.5f);

		const sb::Color color(1, 1, 1, 0.3f);
		m_mesh[index * 6 + 0] = sb::Vertex(edges[0], color, sb::Vector2f(0, 0));
		m_mesh[index * 6 + 1] = sb::Vertex(edges[0], color, sb::Vector2f(0, 0));
		m_mesh[index * 6 + 2] = sb::Vertex(edges[1], color, sb::Vector2f(1, 0));
		m_mesh[index * 6 + 3] = sb::Vertex(edges[2], color, sb::Vector2f(0, 1));
		m_mesh[index * 6 + 4] = sb::Vertex(edges[3], color, sb::Vector2f(1, 1));
		m_mesh[index * 6 + 5] = sb::Vertex(edges[3], color, sb::Vector2f(1, 1));
	}

	void randomizeParticle(std::size_t index, const sb::Vector2f& positionRange) {
		sb::Vector2f position = sb::random2D(positionRange.x, positionRange.y);
		sb::Vector2f scale = sb::random(m_particleSizeRange.x, m_particleSizeRange.y);
		float rotation = sb::random(0, 2 * sb::Pi);
		sb::Transform transform(position, scale, rotation);
		setParticle(transform, index);
		m_velocities[index] = sb::random(0.05f, 0.5f) * position.normalized();
	}

	void hideParticle(std::size_t index) {
		m_mesh[index * 6 + 0].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 1].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 2].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 3].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 4].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 5].position = sb::Vector2f(0, 0);
	}

	void scaleParticle(std::size_t index) {
		static const sb::Vector2f positionRange(-0.1f, 0.1f);
		if (m_isGrowing)
			randomizeParticle(index, positionRange);
		else
			hideParticle(index);
	}

	void scaleParticle() {
		scaleParticle(m_updateIndex);
		m_updateIndex++;
		if (m_updateIndex == m_numParticles - 1) {
			m_isGrowing = !m_isGrowing;
			m_updateIndex = 0;
		}
	}

	void scale(float ds) {
		m_elapsed += ds;
		float decayRate = 0.35f;
		float decayInterval = 1 / (m_numParticles * decayRate);
		while (m_elapsed > decayInterval) {
			scaleParticle();
			m_elapsed -= decayInterval;
		}
	}

	void moveParticle(float ds, std::size_t index) {
		m_mesh[index * 6 + 0].position += ds * m_velocities[index];
		m_mesh[index * 6 + 1].position += ds * m_velocities[index];
		m_mesh[index * 6 + 2].position += ds * m_velocities[index];
		m_mesh[index * 6 + 3].position += ds * m_velocities[index];
		m_mesh[index * 6 + 4].position += ds * m_velocities[index];
		m_mesh[index * 6 + 5].position += ds * m_velocities[index];
	}

	void move(float ds) {
		for (std::size_t i = 0; i < m_numParticles; i++)
			moveParticle(ds, i);
	}

public:
	ParticleSystem3(std::size_t numParticles)
		: m_particleSizeRange(0, 0.1f), m_numParticles(numParticles),
		m_mesh(m_numParticles * 6, sb::PrimitiveType::TriangleStrip),
		m_velocities(numParticles), m_updateIndex(0),
		m_isGrowing(true), m_elapsed(0), m_texture(NULL)
	{ }

	void initSimple() {
		static const sb::Vector2f positionRange(-1, 1);
		for (std::size_t i = 0; i < m_numParticles; i++)
			randomizeParticle(i, positionRange);
	}

	inline void setTexture(const sb::Texture& texture) { m_texture = &texture; }

	inline void setParticleSizeRange(float min, float max) { m_particleSizeRange = sb::Vector2f(min, max); }

	void update(float ds) {
		scale(ds);
		move(ds);
	}

	void updateSimple(float ds) {
		move(ds / 10);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();
		drawStates.texture = m_texture;
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), drawStates);
	}
};

void demo3() {
	sb::Window window;
	ParticleSystem3 particles(20000);
	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	particles.setTexture(greenPropulsion);
	particles.setScale(0.1f, 0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particles.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particles);
		window.display();
		printStats();
	}
}

void demo4() {
	sb::Window window;
	ParticleSystem3 particles(2500);
	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	particles.setTexture(greenPropulsion);
	particles.setScale(0.5f, 0.5f);
	particles.initSimple();

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particles.updateSimple(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particles);
		window.display();
		printStats(10);
	}
}

void demo5() {
	sb::Window window;
	ParticleSystem3 particles(2500);
	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	particles.setTexture(greenPropulsion);
	particles.setScale(0.5f, 0.5f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particles.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particles);
		window.display();
		printStats(10);
	}
}

void demo6() {
	float aspect = 360.0f / 640.0f;
	sb::Window window(360, 640);
	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	ParticleSystem3 particles(100);
	particles.setTexture(greenPropulsion);
	particles.setScale(0.1666666f, 0.1666666f * aspect);
	particles.setParticleSizeRange(0.158f, 0.5f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particles.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particles);
		window.display();
		printStats(10);
	}
}


int main(int argc, char* args[])
{
	SDL_Log("Simple particle system Renderer: Build %s %s", __DATE__, __TIME__);
	srand(48);

	demo6();

	// demo5();

	// demo4();

	// demo3();

	// demo2();
	
	// demo1();
}
