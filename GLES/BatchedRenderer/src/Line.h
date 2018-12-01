#pragma once

#include "Drawable.h"

namespace sb
{
	class Line : public Drawable
	{
	public:
		Line(Vector2f position = Vector2f(0, 0))
			 : Drawable(LineMesh, m_transform), m_transform(position)
		{ }

	private:
		static const Mesh LineMesh;

		Transform m_transform;
	};
}