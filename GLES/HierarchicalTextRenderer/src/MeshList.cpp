#include "MeshList.h"
#include <algorithm>

namespace sb
{
	std::vector<Vertex>& MeshList::getVertices()
	{
		if (m_needsUpdate)
			update();
		
		return m_vertices;
	}

	std::vector<GLushort>& MeshList::getIndices()
	{
		if (m_needsUpdate)
			update();

		return m_indices;
	}

	void MeshList::add(const Mesh& mesh)
	{
		m_newVertexCount += mesh.getVertexCount();
		m_newIndexCount += mesh.getIndexCount();
		m_newMeshes.push_back(mesh);
		m_needsUpdate = true;
	}

	void MeshList::update()
	{
		insertMeshes();
		insertVertices();
		insertIndices();

		m_newMeshes.clear();
		m_newVertexCount = 0;
		m_newIndexCount = 0;

		m_needsUpdate = false;
	}

	void MeshList::insertMeshes()
	{
		m_meshes.insert(m_meshes.end(), m_newMeshes.begin(), m_newMeshes.end());
	}

	void MeshList::insertVertices()
	{
		std::size_t position = m_vertices.size();
		m_vertices.resize(m_vertices.size() + m_newVertexCount);

		for (std::size_t i = 0; i < m_newMeshes.size(); i++) {
			const std::vector<Vertex>& vertices = m_newMeshes[i].getVertices();
			std::copy(vertices.begin(), vertices.end(), m_vertices.begin() + position);
			position += vertices.size();
		}
	}

	void MeshList::insertIndices()
	{
		std::size_t position = m_indices.size();
		m_indices.resize(m_indices.size() + m_newIndexCount);

		for (std::size_t i = 0; i < m_newMeshes.size(); i++) {
			const std::vector<GLushort>& indices = m_newMeshes[i].getIndices();
			for (std::size_t j = 0; j < indices.size(); i++)
				m_indices[position + j] = indices[j] + m_indexOffset;

			position += indices.size();
			m_indexOffset += (GLushort)m_newMeshes[i].getVertexCount();
		}
	}
}