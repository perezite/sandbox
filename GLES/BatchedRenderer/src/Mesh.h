#pragma once

#include "GL.h"
#include "Vertex.h"
#include <vector>

namespace sb
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices)
			: m_vertices(vertices)
		{ }

		std::size_t getVertexCount() const { return m_vertices.size(); }

		const Vertex& operator[](std::size_t index) const { return m_vertices[index]; }

	private:
		std::vector<Vertex> m_vertices;
	};
}