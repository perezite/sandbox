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
		{
			calcIndices();
		}

		inline std::size_t getVertexCount() const { return m_vertices.size(); }

		inline const std::vector<Vertex>& getVertices() const { return m_vertices; }

		inline std::size_t getIndexCount() const { return m_indices.size(); }

		inline const std::vector<GLushort>& getIndices() const { return m_indices; }

		inline const Vertex& operator[](std::size_t index) const { return m_vertices[index]; }

		void setPositions(const std::vector<Vector2f>& positions);

	protected:
		void calcIndices();

	private:
		std::vector<Vertex> m_vertices;

		std::vector<GLushort> m_indices;
	};
}