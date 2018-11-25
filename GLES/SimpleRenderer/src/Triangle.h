#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0)
			 : Drawable(TriangleMesh, m_transform), m_transform(position, scale, rotation)
		{ }

	private:
		static const Mesh TriangleMesh;

		Transform m_transform;
	};
}