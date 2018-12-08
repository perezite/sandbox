#pragma once

#include "Drawable.h"

namespace sb
{
	class Rectangle : public Drawable
	{
	public:
		Rectangle(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0)
			: Drawable(RectangleMesh, m_transform), m_transform(position, scale, rotation)
		{ }

	private:
		static const Mesh RectangleMesh;

		Transform m_transform;
	};
}