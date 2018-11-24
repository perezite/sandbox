#include "Transform.h"

namespace sb
{
	Vector2f operator*(const Transform & transform, Vector2f right)
	{
		return Vector2f(transform.scale.x * right.x + transform.position.x, transform.scale.y * right.y + transform.position.y);
	}
}