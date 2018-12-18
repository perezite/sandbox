#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	class Window;

	class Drawable
	{
	public:
		Drawable(const Mesh& mesh, const Transform& transform)
			: m_mesh(mesh), m_transformedMesh(mesh), m_transform(transform)
		{
			setTransform(m_transform);
		}

	public:
		virtual void draw(Window& window);

		inline const Mesh& getMesh() const { return m_mesh; }

		inline Transform& getTransform() { return m_transform; }

		inline const Mesh& getTransformedMesh() const { return m_transformedMesh; }

		void setTransform(const Transform& transform);

		inline void setRotation(float alpha) { m_transform.rotation = alpha; refreshTransform(); }

	protected: 
		void refreshTransform();

	private:
		const Mesh& m_mesh;

		Mesh m_transformedMesh;

		Transform m_transform;
	};
}