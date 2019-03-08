#pragma once
#include "Drawable.h"
#include "Mesh.h"

namespace sb 
{
	class Shape : public Drawable
	{
	public:
		Shape(const Mesh& mesh) 
			: m_mesh(mesh)
		{ }
		
		virtual ~Shape() {};

		inline const Mesh& getMesh() const { return m_mesh; }

		inline const PrimitiveType getPrimitiveType() const { return m_mesh.getPrimitiveType(); }

	private:
		Mesh m_mesh;
	};
}