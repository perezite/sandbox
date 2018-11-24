#pragma once

#include "Vector2f.h"

namespace sb
{
	class Transform
	{
	public:
		Transform(Vector2f position_, Vector2f scale_) 
			: position(position_), scale(scale_)
		{ }

		Transform()
			: scale(Vector2f(1, 1))
		{ }

		Vector2f position;

		Vector2f scale;
	};

	Vector2f operator *(const Transform& transform, Vector2f position);
}