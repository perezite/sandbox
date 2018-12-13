#include "Transform.h"


namespace sb
{
	Vector2f operator*(const Transform & transform, Vector2f right)
	{
		float sa = sinf(transform.rotation);
		float ca = cosf(transform.rotation);
		float sx = transform.scale.x * right.x;
		float sy = transform.scale.y * right.y;

		return Vector2f(
			(ca * sx - sa * sy) + transform.position.x,
			(sa * sx + ca * sy) + transform.position.y);
	}
}