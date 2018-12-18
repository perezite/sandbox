#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0)
			 : Drawable(TriangleMesh, Transform(position, scale, rotation))
		{ }

		static const Mesh TriangleMesh;
	};
}