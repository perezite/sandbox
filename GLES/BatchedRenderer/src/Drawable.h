#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh_, Transform& transform_, Material& material_)
			: mesh(mesh_), transform(transform_), material(material_)
		{ }

		Drawable(const Mesh& mesh_, Transform& transform_)
			: mesh(mesh_), transform(transform_), material(Material())
		{ }

	public:
		const Mesh& mesh;

		Transform& transform;

		Material material;
	};
}