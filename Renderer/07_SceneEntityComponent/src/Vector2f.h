#pragma once

namespace sb
{
	struct Vector2f
	{
		Vector2f(float x_ = 0, float y_ = 0) 
			: x(x_), y(y_)
		{ }

		float x, y;
	};

	Vector2f operator+(const Vector2f& vector, const Vector2f& transform);
}
