#pragma once

#include "GL.h"
#include "Vertex.h"

#include <vector>

namespace sb
{
	namespace renderer2
	{
		class Mesh
		{
		public:
			Mesh(const std::vector<Vertex>& vertices)
				: m_vertices(vertices)
			{
				computeIndices();
			}

			std::size_t getVertexCount() const { return m_vertices.size(); }

			const Vertex& operator[](std::size_t index) const { return m_vertices[index]; }

			std::size_t getIndexCount() const { return m_indices.size(); }

			const std::vector<GLuint>& getIndices() const { return m_indices; }

		protected: 
			void computeIndices();

		private:
			std::vector<Vertex> m_vertices;

			std::vector<GLuint> m_indices;
		};
	}
}