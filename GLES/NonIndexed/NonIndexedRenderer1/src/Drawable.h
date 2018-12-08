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

	public:
		const Mesh& mesh;

		Transform& transform;
	};
}