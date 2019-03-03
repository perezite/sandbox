#pragma once

#include "Vector2f.h"
#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace sb
{
	static float ToRadian = 2.0f * float(M_PI) / 360.0f;

	class Transform
	{
	public:
		static Transform Identity;

		static float ToRadians;

	public:
		Transform();

		Transform(Vector2f position_, Vector2f scale_, float rotation_);

		Transform(float a00, float a01, float a02,
			float a10, float a11, float a12,
			float a20, float a21, float a22);

		inline const float* getMatrix() const { return m_matrix; }

		Transform getTransposed() const;

		const Transform& translate(const Vector2f& position);

		const Transform& scale(const Vector2f& scale);

		const Transform& rotate(const float angle);

		const Transform& apply(const Transform& transform);

		const Vector2f& transform(Vector2f& vector) const;

		const Mesh& transform(Mesh& mesh) const;

	private: 
		float m_matrix[9];
	};

	Transform& operator *=(Transform& left, const Transform& right);
	
	Transform operator *(const Transform& left, const Transform& right);

	const Vector2f& operator*=(Vector2f& vector, const Transform& transform);

	Mesh operator *(const Transform& transform, const Mesh& mesh);
}