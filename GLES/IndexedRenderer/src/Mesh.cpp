#include "Mesh.h"
#include <algorithm>

namespace sb
{
	void Mesh::calcIndices()
	{
		if (m_vertices.size() <= 2) {
			for (GLushort i = 0; i < m_vertices.size(); i++)
				m_indices.push_back(i);
			return;
		}

		for (GLushort i = 2; i < (GLushort)m_vertices.size(); i++) {
			m_indices.push_back(0);
			m_indices.push_back(i - 1);
			m_indices.push_back(i);
		}
	}
}