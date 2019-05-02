#include "Vector2f.h"
#include "Transformable.h"
#include "Body.h"

namespace sb
{
	class Physics 
	{
	public:
		Physics(float aspect)
			: _aspect(aspect), _inverseAspect(1 / aspect), _dragCoefficient(10), _fixedDeltaSeconds(0.02f)
		{ }

		inline void setDragCoefficient(float drag) { _dragCoefficient = drag; }

		void addBody(Body& body);

		void simulate(float ds);

	protected:
		void prepare();

		const Vector2f computeCollisionForce(const Body& current, const Body& other);

		void computeCollisionForces();

		sb::Vector2f computeBoundaryForce(const Body& fruit);

		void computeBoundaryForces();

		void computeDragForces();

		void computeForces();

		void moveBodies(Body& body, const sb::Vector2f& force, float ds);

		void moveBodies(float ds);

		void step(float ds);

	private:
		std::vector<Body*> _bodies;

		std::vector<sb::Vector2f> _forces;

		float _aspect;

		float _inverseAspect;

		float _dragCoefficient;

		float _fixedDeltaSeconds;
	};
}
