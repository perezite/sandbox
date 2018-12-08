#include "Mesh.h"

namespace sb
{
	void Mesh::computeIndices()
	{
		for (std::size_t i = 0; i < m_vertices.size() - 2; i++) {
			m_indices.push_back((GLuint)i);
			m_indices.push_back((GLuint)i + 1);
			m_indices.push_back(m_vertices.size() - 1);
		}
	}
}