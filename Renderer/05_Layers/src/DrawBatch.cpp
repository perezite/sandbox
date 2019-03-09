#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb
{
	void DrawBatch::begin(DrawTarget& target)
	{
		if (m_target != NULL)
			SB_ERROR() << "The draw batch cycle must be finished by calling DrawBatch::end()" << std::endl;

		m_buffer.clear();
		m_target = &target;
	}

	void DrawBatch::draw(Drawable* drawable, const RenderStates& states)
	{
		drawable->draw(*this, states);
	}

	void DrawBatch::draw(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType, const RenderStates& states)
	{
		m_unbatchedDrawCalls++;

		assertBufferSize(vertices);

		if (mustFlush(vertices, primitiveType)) {
			flush();
		}

		m_currentPrimitiveType = primitiveType;
		bufferVertices(vertices, primitiveType, states.transform);
	}

	void DrawBatch::end()
	{
		if (m_buffer.size() > 0)
			flush();
		m_target = NULL;
	}

	inline void DrawBatch::assertBufferSize(const std::vector<Vertex>& vertices)
	{
		if (vertices.size() > m_buffer.capacity())
			SB_ERROR() << "The DrawBatch buffer size is too small for the given drawable. Please specify a larger buffer size in the constructor" << std::endl;
	}

	inline bool DrawBatch::mustFlush(const std::vector<Vertex>& vertices, PrimitiveType primitiveType)
	{
		if (m_buffer.empty())
			return false;

		bool bufferTooSmall = m_buffer.size() + vertices.size() > m_buffer.capacity();
		bool primitiveTypeChanged = primitiveType != m_currentPrimitiveType;

		return bufferTooSmall || primitiveTypeChanged;
	}

	void DrawBatch::flush() 
	{
		m_batchedDrawCalls++;

		m_target->draw(m_buffer, m_currentPrimitiveType);
		m_buffer.clear();
	}

	void DrawBatch::bufferVertices(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType, const Transform& transform)
	{
		std::vector<Vertex> transformedVertices(vertices);
		transformVertices(transformedVertices, transform);
		
		if (primitiveType == PrimitiveType::Triangles)
			bufferTriangles(transformedVertices);
		else if (primitiveType == PrimitiveType::TriangleStrip)
			bufferTriangleStrip(transformedVertices);
		else
			SB_ERROR() << "The primitive type " << (int)primitiveType << " is not eligible for batching" << std::endl;
	}

	inline void DrawBatch::transformVertices(std::vector<Vertex>& vertices, const Transform& transform) 
	{
		for (std::size_t i = 0; i < vertices.size(); i++) {
			vertices[i].position *= transform;
		}
	}

	inline void DrawBatch::bufferTriangles(const std::vector<Vertex>& vertices) 
	{
		m_buffer.insert(m_buffer.end(), vertices.begin(), vertices.end());
	}

	inline void DrawBatch::bufferTriangleStrip(const std::vector<Vertex>& vertices)
	{
		m_buffer.push_back(vertices[0]);
		m_buffer.insert(m_buffer.end(), vertices.begin(), vertices.end());
		m_buffer.push_back(vertices[vertices.size() - 1]);
	}
}
