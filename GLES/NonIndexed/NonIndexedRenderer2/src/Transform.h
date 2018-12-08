#pragma once

#include "Vector2f.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace sb
{
	static float ToRadian = 2.0f * float(M_PI) / 360.0f;

	class Transform
	{
	public:
		Transform(Vector2f position_ = Vector2f(0, 0), Vector2f scale_ = Vector2f(1, 1), float rotation_ = 0) 
			: position(position_), scale(scale_), rotation(rotation_)
		{ }

		Vector2f position;

		Vector2f scale;

		float rotation;
	};

	Vector2f operator *(const Transform& transform, Vector2f position);
}