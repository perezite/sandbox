#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable(const Mesh& mesh) : m_mesh(mesh) { }

		void setPosition(float x, float y) { setPosition(sb::Vector2f(x, y)); }

		void setPosition(sb::Vector2f position) { m_transform.position = position; }

		void setRotation(float angle) { m_transform.rotation = angle; }

		void setScale(float x, float y) { setScale(sb::Vector2f(x, y)); }

		void setScale(sb::Vector2f scale) { m_transform.scale = scale; }

		const std::vector<Vertex>& getVertices() { return m_mesh.getVertices(); }

	private:
		Mesh m_mesh;

		Transform m_transform;
	};
}