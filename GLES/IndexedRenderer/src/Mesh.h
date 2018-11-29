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

		inline const Vertex& operator[](std::size_t index) const { return m_vertices[index]; }

		inline std::size_t getIndexCount() const { return m_indices.size(); }

		inline const std::vector<GLushort>& getIndices() const { return m_indices; }

		inline const GLushort getLargestIndex() const { return m_largestIndex; }

	protected:
		void calcIndices();

	private:
		std::vector<Vertex> m_vertices;

		std::vector<GLushort> m_indices;

		GLushort m_largestIndex;
	};
}