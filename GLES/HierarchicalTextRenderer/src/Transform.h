#pragma once

#include "Vector2f.h"
#include "Mesh.h"

namespace sb
{
	static float ToRadian = 2.0f * float(M_PI) / 360.0f;

	class Transform
	{
	public:
		Transform();

		Transform(Vector2f position, Vector2f scale, float rotation);

		Transform(float a00, float a01, float a02,
				  float a10, float a11, float a12,
				  float a20, float a21, float a22);

		const Vector2f getTransformed(const Vector2f& point) const;

		const Mesh getTransformed(const Mesh& mesh) const;

		const float* getMatrix() const;

		const Transform& translate(const Vector2f& position);

		const Transform& scale(const Vector2f& scale);

		const Transform& rotate(const float angle);

		const Transform& apply(const Transform& transform);

	private: 
		float m_matrix[9];
	};

	const Transform operator*(const Transform& left, const Transform& right);

	const Vector2f operator*(const Transform& transform, const Vector2f& point);

	const Mesh operator*(const Transform& transform, const Mesh& mesh);
}
