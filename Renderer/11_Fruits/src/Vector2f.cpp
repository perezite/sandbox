#include "Vector2f.h"
#include <math.h>

namespace sb
{
	Vector2f operator-(const Vector2f & vector)
	{
		return Vector2f();
	}

	float Vector2f::getLength()
	{
		return sqrt(x * x + y * y);
	}

	Vector2f Vector2f::normalized()
	{
		float inverseLength = 1 / getLength();
		return inverseLength * (*this);
	}

	Vector2f operator+(const Vector2f& left, const Vector2f& right)
	{
		return Vector2f(left.x + right.x, left.y + right.y);
	}

	Vector2f operator-(const Vector2f & left, const Vector2f& right)
	{
		return Vector2f(left.x - right.x, left.y - right.y);
	}

	Vector2f operator*(float scalar, const Vector2f & vector)
	{
		return Vector2f(scalar * vector.x, scalar * vector.y);
	}

	const Vector2f & operator+=(Vector2f& left, const Vector2f& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}
}
