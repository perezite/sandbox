#pragma once

#include "Vector2f.h"

namespace sb
{
	static float ToRadian = 2.0f * float(M_PI) / 360.0f;

	class Transform
	{
	public:
		Transform();

		Transform(Vector2f position, Vector2f scale, float rotation);

		Vector2f transformPoint(const Vector2f& point) const;

	private: 
		float m_matrix[9];
	};

	Vector2f operator *(const Transform& transform, const Vector2f& point);
}