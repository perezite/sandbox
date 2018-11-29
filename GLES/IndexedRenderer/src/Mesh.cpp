#include "Mesh.h"
#include <algorithm>

namespace sb
{
	void Mesh::calcIndices()
	{
		for (GLushort i = 2; i < (GLushort)m_vertices.size(); i++) {
			m_indices.push_back(0);
			m_indices.push_back(i - 1);
			m_indices.push_back(i);
		}

		m_largestIndex = *std::max_element(m_indices.begin(), m_indices.end());
	}
}