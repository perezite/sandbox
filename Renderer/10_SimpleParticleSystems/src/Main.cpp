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
	const sb::Texture* m_texture;
	std::size_t m_updateIndex;
	bool m_isShrinking;
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
		sb::Vector2f position = sb::random2D(-1, 1);
		sb::Vector2f scale = sb::random(m_scaleRange.x, m_scaleRange.y);
		float rotation = sb::random(0, 2 * sb::Pi);
		sb::Transform transform(position, scale, rotation);
		setParticle(transform, index);
	}

	void init() {
		for (std::size_t i = 0; i < m_numParticles; i++) 
			randomizeParticle(i);
	}

	void hideParticle(std::size_t index) {
		m_mesh[index * 6 + 0].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 1].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 2].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 3].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 4].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 5].position = sb::Vector2f(0, 0);
	}

	void updateParticle(std::size_t index) {
		if (m_isShrinking)
			hideParticle(index);
		else
			randomizeParticle(index);
	}

	void updateParticle() {
		updateParticle(m_updateIndex);
		m_updateIndex++;
		if (m_updateIndex == m_numParticles - 1) {
			m_isShrinking = !m_isShrinking;
			m_updateIndex = 0;
		}
	}

public:
	ParticleSystem(std::size_t numParticles)
		: m_scaleRange(0.02f, 0.04f), m_numParticles(numParticles), m_mesh(m_numParticles * 6, 
			sb::PrimitiveType::TriangleStrip), m_texture(NULL), m_updateIndex(0),
			m_isShrinking(true), m_elapsed(0)
	{
		init();
	}

	void update(float ds) {
		m_elapsed += ds;
		while (m_elapsed > 0.001f) {
			updateParticle();
			m_elapsed -= 0.001f;
		}
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();
		drawStates.texture = m_texture;
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), drawStates);
	}
};

float getDeltaSeconds()
{
	static sb::Stopwatch sw;
	static float lastElapsed = 0;
	float elapsed = sw.getElapsedSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void demo1() {
	sb::Window window;
	ParticleSystem particles(5000);

	float elapsed = 0;
	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		particles.update(getDeltaSeconds());

		window.clear();
		window.draw(particles);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple particle system Renderer: Build %s %s", __DATE__, __TIME__);
	srand(48);

	demo1();
}
