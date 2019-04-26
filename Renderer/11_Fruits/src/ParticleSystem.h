#include "Drawable.h"

namespace sb
{
	class ParticleSystem : public Drawable 
	{
	public:
		ParticleSystem(std::size_t numParticles, float timeOffset = 0);

		inline void setTexture(const sb::Texture& texture) { m_texture = &texture; }

		inline void setParticleSizeRange(float min, float max) { m_particleSizeRange = sb::Vector2f(min, max); }

		void update(float ds);

		virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

	protected:
		void scaleParticles(float ds);

		void scaleParticle();

		void scaleParticle(std::size_t index);

		void randomizeParticle(std::size_t index, const sb::Vector2f& positionRange);

		void setParticle(sb::Transform& transform, std::size_t index);

		void hideParticle(std::size_t index);

		void moveParticles(float ds);

		void moveParticle(float ds, std::size_t index);

	private:
		const sb::Texture* m_texture;
		
		sb::Mesh m_mesh;

		std::size_t m_numParticles;

		sb::Vector2f m_particleSizeRange;

		std::vector<sb::Vector2f> m_velocities;

		std::size_t m_updateIndex;

		bool m_isGrowing;

		float m_elapsed;
	};
}