#pragma once

namespace sb
{
	struct Vector2f
	{
		Vector2f(float x_ = 0, float y_ = 0) 
			: x(x_), y(y_)
		{ }

		const Vector2f& add(Vector2f other)
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		float x, y;
	};

	inline const Vector2f& operator+=(Vector2f& left, const Vector2f& right)
	{
		return left.add(right);
	}
}
