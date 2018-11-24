#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle(Vector2f position, Vector2f scale)
			 : m_transform(position, scale), Drawable(TriangleMesh, m_transform)
		{ }

		Triangle()
			: Drawable(TriangleMesh, m_transform)
		{ }

	public:
		static const Mesh TriangleMesh;

		Transform m_transform;
	};
}