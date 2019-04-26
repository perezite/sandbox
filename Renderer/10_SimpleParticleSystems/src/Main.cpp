#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include "Triangle.h"
#include "Texture.h"
#include "Sprite.h"
#include "Math.h"
#include "Stopwatch.h"
#include "ParticleSystem.h"

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
	ParticleSystem3(std::size_t numParticles, float timeOffset = 0)
		: m_particleSizeRange(0, 0.1f), m_numParticles(numParticles),
		m_mesh(m_numParticles * 6, sb::PrimitiveType::TriangleStrip),
		m_velocities(numParticles), m_updateIndex(0),
		m_isGrowing(true), m_elapsed(timeOffset), m_texture(NULL)
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

class Backdrop : public sb::Drawable {
	sb::Texture m_texture;
	sb::Sprite m_sprite;

public:
	Backdrop()
		: m_texture("Textures/Backdrop.png"), m_sprite(m_texture)
	{ 
		float scale = 2.0f / 3.0f;
		m_sprite.setScale(scale, scale);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();

		float step = 2.0f / 3.0f;
		for (std::size_t i = 0; i < 3; i++) {
			for (std::size_t j = 0; j < 3; j++) {
				float horizontalPos = -1 + i * step + 0.5f * step;
				float verticalPos = -1 + j * step + 0.5f * step;
				m_sprite.setPosition(horizontalPos, verticalPos);
				target.draw(m_sprite, drawStates);
			}
		}
	}

};

void initParticleSystems(std::vector<ParticleSystem3>& result, std::size_t numSystems,
	std::size_t numParticlesPerSystem, float aspect, const sb::Texture& texture)
{
	for (std::size_t i = 0; i < numSystems; i++) {
		ParticleSystem3 system(numParticlesPerSystem, sb::random(0, 0));
		system.setScale(0.1666666f, 0.1666666f * aspect);
		system.setParticleSizeRange(0.158f, 0.5f);
		system.setPosition(sb::random2D(-0.95f, 0.95f));
		system.setTexture(texture);

		result.push_back(system);
	}
}

void initSpriteBatch(std::vector<sb::Sprite>& sprites, std::size_t startIndex, std::size_t count, sb::Texture& texture, float aspect) {
	for (std::size_t i = 0; i < count; i++) {
		sprites[startIndex + i].setTexture(&texture);
		sprites[startIndex + i].setPosition(sb::random2D(-1, 1));
		float scale = sb::random(0.05f, 0.13f);
		sprites[startIndex + i].setScale(scale, scale * aspect);
	}
}

void initSprites(std::vector<sb::Sprite>& sprites, std::vector<sb::Texture>& textures, float aspect) {
	sprites.resize(140);
	std::size_t batchSize = 14;
	std::size_t textureIndex = 0;
	for (std::size_t i = 0; i < sprites.size(); i += batchSize) {
		initSpriteBatch(sprites, i, batchSize, textures[textureIndex], aspect);
		textureIndex = ++textureIndex % textures.size();
	}
}

void update6(std::vector<ParticleSystem3>& particleSystems, float ds) {
	for (std::size_t i = 0; i < particleSystems.size(); i++)
		particleSystems[i].update(ds);
}

void draw6(Backdrop& backdrop, std::vector<sb::Sprite>& sprites, std::vector<ParticleSystem3>& particleSystems, sb::DrawBatch& batch, sb::DrawTarget& window) {
	batch.draw(backdrop);
	for (std::size_t i = 0; i < sprites.size(); i++)
		batch.draw(sprites[i]);
	window.draw(batch);
	for (std::size_t i = 0; i < particleSystems.size(); i++)
		window.draw(particleSystems[i]);
}
		  
void demo6() {
	float width = 360;
	float height = 640;
	sb::Window window((int)width, (int)height);
	sb::DrawBatch batch(8192);

	float aspect = width / height;
	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	std::vector<sb::Texture> spriteTextures(5);
	spriteTextures[0].loadFromAsset("Textures/CyanBlock.png");
	spriteTextures[1].loadFromAsset("Textures/GreenBlock.png");
	spriteTextures[2].loadFromAsset("Textures/PurpleBlock.png");
	spriteTextures[3].loadFromAsset("Textures/RedBlock.png");
	spriteTextures[4].loadFromAsset("Textures/YellowBlock.png");
	Backdrop backdrop;
	std::vector<ParticleSystem3> particleSystems;
	std::vector<sb::Sprite> sprites;
	initParticleSystems(particleSystems, 15, 100, aspect, greenPropulsion);
	initSprites(sprites, spriteTextures, aspect);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		update6(particleSystems, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		draw6(backdrop, sprites, particleSystems, batch, window);

		window.display();
		printStats(10);
	}
}

void demo7() {
	float width = 360; 
	float height = 640;
	float aspect = width / height;
	sb::Window window((int)width, (int)height);

	sb::Texture greenPropulsion("Textures/GreenPropulsion.png");
	sb::ParticleSystem particleSystem(128);
	particleSystem.setScale(0.15f, 0.15f * aspect);
	particleSystem.setParticleSizeRange(0.25f, 0.6f);
	particleSystem.setTexture(greenPropulsion);

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

int main(int argc, char* args[])
{
	SDL_Log("Simple particle system Renderer: Build %s %s", __DATE__, __TIME__);
	srand(48);

	demo7();

	// demo6();

	// demo5();

	// demo4();

	// demo3();

	// demo2();
	
	// demo1();
}
