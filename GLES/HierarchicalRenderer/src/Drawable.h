#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

namespace sb
{
	class Window;

	class Drawable
	{
	public:
		Drawable(const Mesh& mesh, const Transform& transform, Shader* shader = Shader::getDefault())
			: m_mesh(mesh), m_transformedMesh(mesh), m_transform(transform), m_material(Material(shader)), m_layer(0)
		{
			setTransform(m_transform);
		}

	public:
		virtual void draw(Window& window);

		inline const Mesh& getMesh() const { return m_mesh; }

		inline Transform& getTransform() { return m_transform; }

		inline Material& getMaterial() { return m_material; }

		inline const Mesh& getTransformedMesh() const { return m_transformedMesh; }

		inline const int getLayer() const { return m_layer; }

		void setTransform(const Transform& transform);

		inline void setRotation(float alpha) { m_transform.rotation = alpha; refreshTransform(); }

		inline void setMaterial(const Material& material) { m_material = material; }

		inline void setLayer(int layer) { m_layer = layer; }

	protected: 
		void refreshTransform();

	private:
		const Mesh& m_mesh;

		Mesh m_transformedMesh;

		Transform m_transform;

		Material m_material;

		int m_layer;
	};
}