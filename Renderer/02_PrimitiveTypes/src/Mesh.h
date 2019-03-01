#pragma once

#include "GL.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include <vector>

namespace sb
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType) 
			: m_vertices(vertices), m_primitiveType(primitiveType)
		{ }

		Mesh() { }

		void setVertices(std::vector<Vertex>& vertices) { m_vertices = vertices; }

		void setPrimitiveType(const PrimitiveType primitiveType) { m_primitiveType = primitiveType; }

		std::size_t getVertexCount() const { return m_vertices.size(); }

		const std::vector<Vertex>& getVertices() { return m_vertices; }

		const PrimitiveType& getPrimitiveType() { return m_primitiveType; }

		const Vertex& operator[](std::size_t index) const { return m_vertices[index]; }

	private:
		std::vector<Vertex> m_vertices;

		PrimitiveType m_primitiveType;
	};
}