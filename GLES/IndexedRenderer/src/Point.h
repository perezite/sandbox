#pragma once

#include "Drawable.h"

namespace sb
{
	class Point : public Drawable
	{
	public:
		Point(Vector2f position = Vector2f(0, 0))
			: Drawable(PointMesh, m_transform)
		{ }

	private:
		static const Mesh PointMesh;

		Transform m_transform;
	};
}