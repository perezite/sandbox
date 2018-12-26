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
			: m_transformNeedsUpdate(true), m_position(0, 0), m_scale(1, 1), m_rotation(0), m_mesh(mesh), m_transformedMesh(mesh), m_parent(NULL)
		{ }

		virtual void draw(Window& window) = 0;

		inline const Transform& getTransform() const { return m_transform; }

		const Vector2f& getPosition() const { return m_position; };

		void setParent(Drawable& parent) { m_parent = &parent; }

		void setPosition(const Vector2f& position);

		inline void setPosition(float x, float y) { setPosition(Vector2f(x, y)); }

		void setScale(const Vector2f& scale);

		inline void setScale(float x, float y) { setScale(Vector2f(x, y)); }

		void setRotation(const float angle);

	protected:
		Mesh& getTransformedMesh();

		void updateTransform();

		void transformMesh();

	private:
		Transform m_transform;

		bool m_transformNeedsUpdate;

		Vector2f m_position;

		Vector2f m_scale;

		float m_rotation;

		Mesh m_mesh;

		Mesh m_transformedMesh;

		Drawable* m_parent;
	};
}