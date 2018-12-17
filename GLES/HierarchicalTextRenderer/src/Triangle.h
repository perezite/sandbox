#pragma once

#include "Drawable.h"
#include "Material.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0, Shader* shader = Shader::getDefault())
			 : Drawable(TriangleMesh, Transform(position, scale, rotation), shader)
		{ }

		static const Mesh TriangleMesh;
	};
}