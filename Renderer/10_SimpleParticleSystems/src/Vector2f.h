#pragma once

namespace sb
{
	struct Vector2f
	{
		Vector2f()
			: x(0), y(0)
		{ }

		Vector2f(float s_)
			: x(s_), y(s_)
		{ }


		Vector2f(float x_, float y_) 
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
