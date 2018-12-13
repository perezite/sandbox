#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh, Transform& transform, Shader* shader = Shader::getDefault())
			: m_mesh(mesh), transform(transform), m_material(Material(shader))
		{ }

	public:
		inline const Mesh& getMesh() const { return m_mesh; }

		inline const Transform& getTransform() const { return transform; }

		inline const Material& getMaterial() const { return m_material; }

		inline void setMaterial(const Material& material) { m_material = material; }

	private:
		const Mesh& m_mesh;

		Transform& transform;

		Material m_material;
	};
}