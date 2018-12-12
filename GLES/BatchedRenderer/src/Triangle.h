#pragma once

#include "Drawable.h"
#include "Material.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0, Shader* shader = Shader::getDefault())
			 : Drawable(TriangleMesh, m_transform, shader), m_transform(position, scale, rotation)
		{ }
		
		inline void setRotation(float omega) { m_transform.rotation = omega; }
	private:
		static const Mesh TriangleMesh;

		Transform m_transform;
	};
}