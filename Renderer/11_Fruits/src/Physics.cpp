#include "Physics.h"
#include "Vector2f.h"
#include <vector>
#include <algorithm>

namespace sb {
	void Physics::prepare() 
	{
		_forces.clear();
		_forces.resize(_bodies.size());
	}

	const sb::Vector2f Physics::computeCollisionForce(const Body& current, const Body& other) 
	{
		const sb::Vector2f currentScale = current.getScale();
		const sb::Vector2f otherScale = other.getScale();
		const float leftRadius = (currentScale.x + currentScale.y) / 4.0f;
		const float rightRadius = (otherScale.x + otherScale.y) / 4.0f;
		sb::Vector2f distance = other.getPosition() - current.getPosition();
		float distanceLength = distance.getLength();
		float collisionDistance = distanceLength - leftRadius - rightRadius;

		return collisionDistance < 0 ?
			collisionDistance * 1000 * distance.normalized() :
			sb::Vector2f(0, 0);
	}

	void Physics::computeCollisionForces() 
	{
		for (std::size_t i = 0; i < _bodies.size(); i++) {
			for (std::size_t j = i + 1; j < _bodies.size(); j++) {
				const Body& current = *_bodies[i];
				const Body& other = *_bodies[j];
				const sb::Vector2f& force = computeCollisionForce(current, other);
				_forces[i] += force;
				_forces[j] += -force;
			}
		}
	}

	sb::Vector2f Physics::computeBoundaryForce(const Body& fruit) 
	{
		const float radius = (fruit.getScale().x + fruit.getScale().y) / 4.0f;
		const sb::Vector2f position = fruit.getPosition();
		sb::Vector2f force;

		float left = position.x - radius;
		float top = position.y + radius;
		float right = position.x + radius;
		float bottom = position.y - radius;

		if (left < -1)
			force += (-1 - left) * sb::Vector2f(1, 0);
		if (top > 1 * _inverseAspect)
			force += (top - 1 * _inverseAspect) * sb::Vector2f(0, -1);
		if (right > 1)
			force += (right - 1) * sb::Vector2f(-1, 0);
		if (bottom < -1 * _inverseAspect)
			force += (-1 * _inverseAspect - bottom) * sb::Vector2f(0, 1);

		return 1000 * force;
	}

	void Physics::computeBoundaryForces() 
	{
		for (std::size_t i = 0; i < _bodies.size(); i++)
			_forces[i] += computeBoundaryForce(*_bodies[i]);
	}


	void Physics::computeDragForces() 
	{
		for (std::size_t i = 0; i < _bodies.size(); i++)
			_forces[i] -= _dragCoefficient * _bodies[i]->velocity;
	}

	void Physics::computeForces() 
	{
		computeCollisionForces();
		computeBoundaryForces();
		computeDragForces();
	}

	void Physics::moveBodies(Body& body, const sb::Vector2f& force, float ds) {
		body.velocity += ds * force;
		sb::Vector2f position = body.getPosition();
		position += ds * body.velocity;
		body.setPosition(position);
	}

	void Physics::moveBodies(float ds) {
		for (std::size_t i = 0; i < _bodies.size(); i++)
			moveBodies(*_bodies[i], _forces[i], ds);
	}

	void Physics::step(float ds) {
		prepare();
		computeForces();
		moveBodies(ds);
		_bodies.clear();
	}

	void Physics::addBody(Body& body) 
	{
		_bodies.push_back(&body);
	}

	void Physics::simulate(float ds) 
	{
		float sum = 0;
		float remaining = ds;
		do {
			float timestep = std::min(remaining, _fixedDeltaSeconds);
			step(timestep);
			remaining -= _fixedDeltaSeconds;
			sum += timestep;
		} while (remaining > 0);
	}
}
