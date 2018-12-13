#include "DrawBatch.h"

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
			m_indices[count + i] = indices[i] + m_indexOffset;

		m_indexOffset += (GLushort)drawable->getMesh().getVertexCount();	
		m_vertexCount += drawable->getMesh().getVertexCount();
	}

	void DrawBatch::calcVertices(std::vector<Vertex>& result)
	{
		std::size_t count = 0;
		result.resize(m_vertexCount);

		for (std::size_t i = 0; i < m_drawables.size(); i++) {
			const std::vector<Vertex>& vertices = m_drawables[i]->getMesh().getVertices();
			for (std::size_t j = 0; j < vertices.size(); j++) {
				result[count + j].position = m_drawables[i]->getTransform() * vertices[j].position;
				result[count + j].color = vertices[j].color;
			}

			count += m_drawables[i]->getMesh().getVertexCount();
		}
	}
}