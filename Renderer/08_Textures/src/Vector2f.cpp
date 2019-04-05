#include "Vector2f.h"

namespace sb
{
	Vector2f operator+(const Vector2f& left, const Vector2f& right)
	{
		return Vector2f(left.x + right.x, left.y + right.y);
	}
}
