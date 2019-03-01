#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh) : 
			m_mesh(mesh), m_transformNeedsUpdate(true), m_position(0, 0), m_scale(1, 1), m_rotation(0)
		{ }

		const std::vector<Vertex>& getVertices() { return m_mesh.getVertices(); }

		const PrimitiveType getPrimitiveType() { return m_mesh.getPrimitiveType(); }

		const Transform& getTransform();

		inline void setPosition(float x, float y) { setPosition(sb::Vector2f(x, y)); }

		void setPosition(const sb::Vector2f& position);

		inline void setScale(float x, float y) { setScale(sb::Vector2f(x, y)); }

		void setScale(const sb::Vector2f& scale);

		void setRotation(float radians);

	protected:
		void updateTransform();

	private:
		Mesh m_mesh;

		Transform m_transform;

		bool m_transformNeedsUpdate;

		Vector2f m_position;

		Vector2f m_scale;

		float m_rotation;
	};
}