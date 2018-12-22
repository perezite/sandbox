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

		void setPosition(const Vector2f& position);

		void setScale(const Vector2f& scale);

		void setRotation(const float rotation);

		Vector2f getTransformedPoint(const Vector2f& point) const;

		Mesh getTransformedMesh(const Mesh& mesh) const;

	protected:
		void refresh() const;

	private: 
		mutable float m_matrix[9];

		mutable bool m_needsRefresh;

		Vector2f m_position;

		Vector2f m_scale;

		float m_rotation;
	};

	Vector2f operator *(const Transform& transform, const Vector2f& point);

	Mesh operator *(const Transform& transform, const Mesh& mesh);
}