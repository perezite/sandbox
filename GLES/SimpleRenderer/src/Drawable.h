#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh_, Transform& transform_)
			: mesh(mesh_), transform(transform_)
		{ }

		inline void setPosition(const Vector2f position) { transform.position = position; }

		inline void setScale(const Vector2f scale) { transform.scale = scale; }

	public:
		const Mesh& mesh;

		Transform& transform;
	};
}