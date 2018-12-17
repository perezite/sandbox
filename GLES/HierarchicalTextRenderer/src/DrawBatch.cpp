#include "DrawBatch.h"
#include <algorithm>

namespace sb
{
	DrawBatch::~DrawBatch()
	{
		for (std::size_t i = 0; i < m_drawables.size(); i++)
			delete m_drawables[i];
	}

	void DrawBatch::insert(Drawable* drawable)
	{
		const std::vector<GLushort>& indices = drawable->getMesh().getIndices();
		std::size_t count = m_indices.size();
		m_indices.resize(m_indices.size() + indices.size());
		
		for (std::size_t i = 0; i < indices.size(); i++)
			m_indices[count + i] = indices[i] + m_vertexCount;

		m_vertexCount += (GLushort)drawable->getMesh().getVertexCount();
	}

	void DrawBatch::calcVertices(std::vector<Vertex>& result)
	{
		std::size_t count = 0;
		result.resize(m_vertexCount);

		for (std::size_t i = 0; i < m_drawables.size(); i++) {
			const Mesh& mesh = m_drawables[i]->getTransformedMesh();
			const std::vector<Vertex>& vertices = mesh.getVertices();
			std::copy(vertices.begin(), vertices.end(), result.begin() + count);
			count += vertices.size();
		}
	}
}