#pragma once

namespace sb
{
	struct Vector2f
	{
		Vector2f(float x_ = 0, float y_ = 0) 
			: x(x_), y(y_)
		{ }

		float x, y;

		float getLength();

		Vector2f normalized();
	};

	Vector2f operator+(const Vector2f& left, const Vector2f& right);

	Vector2f operator-(const Vector2f& left, const Vector2f& right);

	Vector2f operator*(float scalar, const Vector2f& vector);
}
