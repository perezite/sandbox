#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh, const Transform& transform, Shader* shader = Shader::getDefault())
			: m_mesh(mesh), m_transformedMesh(mesh), m_transform(transform), m_material(Material(shader))
		{
			setTransform(m_transform);
		}

	public:
		inline const Mesh& getMesh() const { return m_mesh; }

		inline Transform& getTransform() { return m_transform; }

		inline const Material& getMaterial() const { return m_material; }

		inline const Mesh& getTransformedMesh() const { return m_transformedMesh; }

		void setTransform(const Transform& transform);

		inline void setMaterial(const Material& material) { m_material = material; }

	private:
		const Mesh& m_mesh;

		Mesh m_transformedMesh;

		Transform m_transform;

		Material m_material;
	};
}