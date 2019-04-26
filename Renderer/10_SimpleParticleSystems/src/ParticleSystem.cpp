#include "ParticleSystem.h"
#include "DrawTarget.h"
#include "Math.h"

namespace sb
{
	ParticleSystem::ParticleSystem(std::size_t numParticles, float timeOffset)
		: m_texture(NULL), m_mesh(numParticles * 6, sb::PrimitiveType::TriangleStrip),
			m_numParticles(numParticles), m_particleSizeRange(0, 0.1f), m_velocities(numParticles), 
			m_updateIndex(0), m_isGrowing(true), m_elapsed(0)
	{ }

	void ParticleSystem::update(float ds)
	{
		scaleParticles(ds);
		moveParticles(ds);
	}

	void ParticleSystem::draw(sb::DrawTarget& target, sb::DrawStates drawStates)
	{
		drawStates.transform *= getTransform();
		drawStates.texture = m_texture;
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), drawStates);
	}

	void ParticleSystem::scaleParticles(float ds)
	{
		m_elapsed += ds;
		float decayRate = 0.35f;
		float decayInterval = 1 / (m_numParticles * decayRate);
		while (m_elapsed > decayInterval) {
			scaleParticle();
			m_elapsed -= decayInterval;
		}
	}

	void ParticleSystem::scaleParticle()
	{
		scaleParticle(m_updateIndex);
		m_updateIndex++;
		if (m_updateIndex == m_numParticles - 1) {
			m_isGrowing = !m_isGrowing;
			m_updateIndex = 0;
		}
	}

	void ParticleSystem::scaleParticle(std::size_t index)
	{
		static const sb::Vector2f positionRange(-0.1f, 0.1f);
		if (m_isGrowing)
			randomizeParticle(index, positionRange);
		else
			hideParticle(index);
	}

	void ParticleSystem::randomizeParticle(std::size_t index, const sb::Vector2f& positionRange)
	{
		sb::Vector2f position = sb::random2D(positionRange.x, positionRange.y);
		sb::Vector2f scale = sb::random(m_particleSizeRange.x, m_particleSizeRange.y);
		float rotation = sb::random(0, 2 * sb::Pi);
		sb::Transform transform(position, scale, rotation);
		setParticle(transform, index);
		m_velocities[index] = sb::random(0.05f, 0.5f) * position.normalized();
	}

	void ParticleSystem::setParticle(sb::Transform& transform, std::size_t index)
	{
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

	void ParticleSystem::hideParticle(std::size_t index)
	{
		m_mesh[index * 6 + 0].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 1].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 2].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 3].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 4].position = sb::Vector2f(0, 0);
		m_mesh[index * 6 + 5].position = sb::Vector2f(0, 0);
	}

	void ParticleSystem::moveParticles(float ds)
	{
		for (std::size_t i = 0; i < m_numParticles; i++)
			moveParticle(ds, i);
	}

	void ParticleSystem::moveParticle(float ds, std::size_t index) 
	{
		m_mesh[index * 6 + 0].position += ds * m_velocities[index];
		m_mesh[index * 6 + 1].position += ds * m_velocities[index];
		m_mesh[index * 6 + 2].position += ds * m_velocities[index];
		m_mesh[index * 6 + 3].position += ds * m_velocities[index];
		m_mesh[index * 6 + 4].position += ds * m_velocities[index];
		m_mesh[index * 6 + 5].position += ds * m_velocities[index];
	}
}