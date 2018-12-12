#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh_, Transform& transform_, Shader* shader = Shader::getDefault())
			: mesh(mesh_), transform(transform_), material(Material(shader))
		{ }

	public:
		const Mesh& mesh;

		Transform& transform;

		Material material;
	};
}