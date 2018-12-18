#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Vector2f.h"
#include "Window.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh)
			: m_mesh(mesh), m_transformedMesh(mesh)
		{ }

		virtual void draw(Window& window);

		void setPosition(const Vector2f& position);

		inline void setPosition(float x, float y) { setPosition(Vector2f(x, y)); }

	protected:
		void computeTransformedMesh();

		const Transform& getTransform();

	private:
		Mesh m_mesh;

		Mesh m_transformedMesh;

		Transform m_transform;

		Vector2f m_position;

		bool m_meshNeedsUpdate;

		bool m_transformNeedsUpdate;
	};
}