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

		void setScale(const Vector2f& scale);

		inline void setScale(float x, float y) { setScale(Vector2f(x, y)); }

		void setRotation(const float rotation);

	protected:
		void computeTransformedMesh();

	private:
		Mesh m_mesh;

		Mesh m_transformedMesh;

		Transform m_transform;

		bool m_meshNeedsUpdate;

		bool m_transformNeedsUpdate;
	};
}