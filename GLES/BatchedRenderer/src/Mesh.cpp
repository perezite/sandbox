#include "Mesh.h"

namespace sb
{
	void Mesh::calcIndices()
	{
		if (m_vertices.size() == 2) m_indices = { 0 };
		if (m_vertices.size() == 2) m_indices = { 0, 1 };

		for (std::size_t i = 0; i < m_vertices.size() - 2; i++) {
			m_indices.push_back((GLushort)i);
			m_indices.push_back((GLushort)(i + 1));
			m_indices.push_back((GLushort)m_vertices.size() - 1);
		}
	}

	void Mesh::setPositions(const std::vector<Vector2f>& positions)
	{
		for (std::size_t i = 0; i < m_vertices.size(); i++) {
			m_vertices[i].position = positions[i];
		}
	}
}