#include "ParticleSystem.h"
#include "DrawTarget.h"
#include "Logger.h"

namespace sb
{
	ParticleSystem::ParticleSystem(const ParticleSystem& other) 
	{
		*this = other;
		this->_emissionShape = other._emissionShape->clone();
		copy(this->_subSystemOnParticleDeath, other._subSystemOnParticleDeath);
		copyVector(this->_subSystems, other._subSystems);
	}

	ParticleSystem::~ParticleSystem()
	{
		for (std::size_t i = 0; i < _subSystems.size(); i++)
			delete _subSystems[i];
		if (_subSystemOnParticleDeath)
			delete _subSystemOnParticleDeath;
		delete _emissionShape;
	}
	
	void ParticleSystem::setEmissionRatePerSecond(float rate)
	{
		_secondsSinceLastEmission = rate == 0 ? 0 : 1 / rate;
		_emissionRatePerSecond = rate;
	}

	void ParticleSystem::addBurst(float emissionTime, std::size_t _numParticles) 
	{
		_bursts.emplace_back(emissionTime, _numParticles);
	}

	void ParticleSystem::setParticleVertexColor(std::size_t index, const Color& color)
	{
		SB_ERROR_IF(index > 4, "Vertex index out of range");
		_particleVertexColors[index] = color;
	}

	void ParticleSystem::setParticleColor(const Color& color)
	{
		std::fill(_particleVertexColors.begin(), _particleVertexColors.end(), color);
	}

	void ParticleSystem::setParticleColorChannelOverLifetime(std::size_t channelIndex, const Tween& particleColorChannelOverLifetime) 
	{
		SB_ERROR_IF(channelIndex > 4, "Color channel index out of range");
		SB_ERROR_IF(particleColorChannelOverLifetime.getDuration() > 1, "Tween duration out of range");
		_particleColorChannelsOverLifetime[channelIndex] = particleColorChannelOverLifetime;
		_hasParticleColorChannelsOverLifetime[channelIndex] = true;
	}

	void ParticleSystem::setParticleScaleOverLifetime(const Tween& particleScaleOverLifetime) 
	{
		SB_ERROR_IF(particleScaleOverLifetime.getDuration() > 1, "Tween duration out of range");
		_particleScaleOverLifetime = particleScaleOverLifetime;
		_hasParticleScaleOverLifetime = true;
	}

	void ParticleSystem::setLifetime(float lifetime)
	{
		_canDie = true;
		_lifetime = lifetime;
	}

	void ParticleSystem::setSubSystemOnParticleDeath(const ParticleSystem& subSystem)
	{
		if (_subSystemOnParticleDeath)
			delete _subSystemOnParticleDeath;
		_subSystemOnParticleDeath = new ParticleSystem(subSystem);
	}

	bool ParticleSystem::isAlive() 
	{
		return !_canDie || _secondsSinceBirth < _lifetime || !_subSystems.empty() || _numActiveParticles > 0;
	}

	void ParticleSystem::reset()
	{
		_secondsSinceLastEmission = _emissionRatePerSecond == 0 ? 0 : 1 / _emissionRatePerSecond;
		_numActiveParticles = 0;
		_secondsSinceBirth = 0;

		for (std::size_t i = 0; i < _particles.size(); i++)
			_particles[i].isActive = false;

		for (std::size_t i = 0; i < _subSystems.size(); i++)
			_subSystemOnParticleDeath[i].reset();

		std::vector<Vertex>& vertices = _mesh.getVertices();
		std::fill(vertices.begin(), vertices.end(), sb::Vertex(sb::Vector2f(0, 0), sb::Color(0, 0, 0, 0)));
	}

	void ParticleSystem::update(float ds)
	{
		_secondsSinceBirth += ds;
	
		updateParticleSystem(ds);
		updateParticles(ds);
		updateSubSystems(ds);

		SB_DEBUG_IF(id == "main", _subSystems.size());
	}

	void ParticleSystem::draw(DrawTarget& target, DrawStates states) {
		if (isAlive()) {
			states.transform *= getTransform();
			states.texture = _texture;
			target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
			drawSubSystems(target, states);
		}
	}

	void ParticleSystem::updateParticleSystem(float ds)
	{
		sb::Vector2f force = -_drag * velocity;
		float torque = -_angularDrag * angularVelocity;
		velocity += force;
		angularVelocity += torque;

		translate(ds * velocity);
		rotate(ds * angularVelocity);
	}

	bool ParticleSystem::isParticleDead(const Particle& particle)
	{
		return particle.secondsSinceBirth > particle.lifetime;
	}
	
	void ParticleSystem::deactivateParticleInMesh(std::size_t meshIndex) 
	{
		_mesh[meshIndex * 6 + 0].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 1].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 2].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 3].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 4].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 5].position = Vector2f(0, 0);
	}

	void ParticleSystem::emitSubSystem(const Particle& particle) 
	{
		ParticleSystem* subSystem = new ParticleSystem(*_subSystemOnParticleDeath);
		subSystem->setPosition(particle.getPosition());
		subSystem->setRotation(particle.getRotation());
		subSystem->velocity = particle.velocity;
		subSystem->angularVelocity = particle.angularVelocity;
		_subSystems.push_back(subSystem);
	}

	void ParticleSystem::removeDeadParticles() 
	{
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive && isParticleDead(_particles[i])) {
				_particles[i].isActive = false;
				_numActiveParticles--;
				deactivateParticleInMesh(i);
				if (_subSystemOnParticleDeath)
					emitSubSystem(_particles[i]);
			}
		}
	}

	bool ParticleSystem::isParticleInactive(const Particle& particle) 
	{
		return !particle.isActive;
	}

	std::size_t ParticleSystem::findAvailableIndex() 
	{
		std::vector<Particle>::iterator it =
			std::find_if(_particles.begin(), _particles.end(), isParticleInactive);
		return std::distance(_particles.begin(), it);
	}

	Vector2f ParticleSystem::getDirection(Particle& particle) 
	{
		bool randomDirection = _hasRandomEmissionDirection || _emissionShape->getBoundingRadius() < 0.0001f;
		return randomDirection ? randomOnCircle(1) : particle.getPosition().normalized();
	}

	void ParticleSystem::initParticle(Particle& particle) 
	{
		particle.setPosition(_emissionShape->random());
		float size = random(_particleSizeRange.x, _particleSizeRange.y);
		particle.setScale(size, size);
		particle.startScale = Vector2f(size, size);
		particle.setRotation(random(_particleRotationRange.x, _particleRotationRange.y));
		Vector2f direction = getDirection(particle);
		particle.velocity = random(_particleSpeedRange.x, _particleSpeedRange.y) * direction;
		particle.angularVelocity = random(_particleAngularVelocityRange.x, _particleAngularVelocityRange.y);
		particle.lifetime = random(_particleLifetimeRange.x, _particleLifetimeRange.y);
		particle.vertexColors = _particleVertexColors;
		particle.startVertexColors = _particleVertexColors;
		particle.isActive = true;
	}

	void ParticleSystem::emitParticle() 
	{
		if (_numActiveParticles == _particles.size())
			return;

		std::size_t availableIndex = findAvailableIndex();
		Particle particle;
		initParticle(particle);
		_particles[availableIndex] = particle;
		_numActiveParticles++;
	}

	void ParticleSystem::emitParticles(float ds) 
	{
		_secondsSinceLastEmission += ds;
		float emissionInterval = 1 / _emissionRatePerSecond;
		while (_secondsSinceLastEmission > emissionInterval) {
			emitParticle();
			_secondsSinceLastEmission -= emissionInterval;
		}
	}

	void ParticleSystem::emitBurst(Burst& burst) 
	{
		for (std::size_t i = 0; i < burst.numParticles; i++)
			emitParticle();

		burst.emitted = true;
	}

	void ParticleSystem::emitBursts(float ds) 
	{
		for (std::size_t i = 0; i < _bursts.size(); i++) {
			if (!_bursts[i].emitted && _secondsSinceBirth >= _bursts[i].emissionTime)
				emitBurst(_bursts[i]);
		}
	}

	Vector2f ParticleSystem::computeParticleForce(Particle& particle) 
	{
		Vector2f dragForce = -_particleDrag * particle.velocity;
		return dragForce;
	}

	float ParticleSystem::computeParticleTorque(Particle& particle)
	{
		float dragTorque = -_angularParticleDrag * particle.angularVelocity;
		return dragTorque;
	}

	void ParticleSystem::updateScale(Particle& particle) 
	{
		if (_hasParticleScaleOverLifetime) {
			float t = getNormalizedSecondsSinceBirth(particle);
			particle.setScale(_particleScaleOverLifetime.value(t) * particle.startScale);
		}
	}

	float ParticleSystem::getNormalizedSecondsSinceBirth(const Particle & particle)
	{
		return particle.secondsSinceBirth / particle.lifetime;
	}

	void ParticleSystem::updateVertexColorChannel(std::size_t channelIndex, float& colorChannel, const float& startColorChannel, float t)
	{
		if (_hasParticleColorChannelsOverLifetime[channelIndex])
			colorChannel = startColorChannel * _particleColorChannelsOverLifetime[channelIndex].value(t);
	}

	void ParticleSystem::updateVertexColor(Color& color, const Color& startColor, Particle& particle) 
	{
		float t = getNormalizedSecondsSinceBirth(particle);

		updateVertexColorChannel(0, color.r, startColor.r, t);
		updateVertexColorChannel(1, color.g, startColor.g, t);
		updateVertexColorChannel(2, color.b, startColor.b, t);
		updateVertexColorChannel(3, color.a, startColor.a, t);
	}

	void ParticleSystem::updateVertexColors(Particle& particle) 
	{
		for (std::size_t i = 0; i < particle.vertexColors.size(); i++)
			updateVertexColor(particle.vertexColors[i], particle.startVertexColors[i], particle);
	}

	void ParticleSystem::transformParticle(Particle& particle, float ds) 
	{
		particle.secondsSinceBirth += ds;

		particle.velocity += ds * computeParticleForce(particle);
		particle.angularVelocity += ds * computeParticleTorque(particle);

		particle.translate(ds * particle.velocity);
		particle.rotate(ds * particle.angularVelocity);

		updateScale(particle);
		updateVertexColors(particle);
	}

	void ParticleSystem::transformParticles(float ds) 
	{
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				transformParticle(_particles[i], ds);
		}
	}

	void ParticleSystem::updateParticleVertices(Particle& particle, std::size_t index) 
	{
		std::vector<Vector2f> edges(4);
		edges[0] = particle.getTransform() * Vector2f(-0.5f, -0.5f);
		edges[1] = particle.getTransform() * Vector2f(0.5f, -0.5f);
		edges[2] = particle.getTransform() * Vector2f(-0.5f, 0.5f);
		edges[3] = particle.getTransform() * Vector2f(0.5f, 0.5f);

		const Color color(1, 0, 0, 1);
		_mesh[index * 6 + 0] = Vertex(edges[0], particle.vertexColors[0], Vector2f(0, 0));
		_mesh[index * 6 + 1] = Vertex(edges[0], particle.vertexColors[0], Vector2f(0, 0));
		_mesh[index * 6 + 2] = Vertex(edges[1], particle.vertexColors[1], Vector2f(1, 0));
		_mesh[index * 6 + 3] = Vertex(edges[2], particle.vertexColors[2], Vector2f(0, 1));
		_mesh[index * 6 + 4] = Vertex(edges[3], particle.vertexColors[3], Vector2f(1, 1));
		_mesh[index * 6 + 5] = Vertex(edges[3], particle.vertexColors[3], Vector2f(1, 1));
	}

	void ParticleSystem::updateMesh(float ds) 
	{
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				updateParticleVertices(_particles[i], i);
		}
	}

	void ParticleSystem::updateParticles(float ds)
	{
		removeDeadParticles();
		if (isAlive()) {
			emitParticles(ds);
			emitBursts(ds);
			transformParticles(ds);
			updateMesh(ds);
		}
	}

	void ParticleSystem::updateSubSystems(float ds)
	{
		deleteFromVector(_subSystems, isParticleSystemDead);

		for (std::size_t i = 0; i < _subSystems.size(); i++)
			_subSystems[i]->update(ds);
	}

	void ParticleSystem::drawSubSystems(DrawTarget& target, DrawStates& states)
	{
		for (std::size_t i = 0; i < _subSystems.size(); i++)
			target.draw(_subSystems[i], states);
	}
}