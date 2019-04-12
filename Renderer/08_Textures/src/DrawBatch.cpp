#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb
{
	std::size_t DrawBatch::Buffer::BatchingThreshold = 20;

	void DrawBatch::draw(Drawable& drawable, const DrawStates& states)
	{
		m_drawCommands.emplace_back(drawable, states);
	}

	void DrawBatch::draw(DrawTarget& target, DrawStates states) 
	{
		states.transform *= getTransform();

		m_buffer.setTarget(target);
		 for (std::size_t i = 0; i < m_drawCommands.size(); i++) 
			 m_buffer.draw(m_drawCommands[i].drawable, m_drawCommands[i].states);

		m_buffer.flush();
		m_drawCommands.clear();
	}

	void DrawBatch::Buffer::draw(const std::vector<Vertex>& vertices, 
		const PrimitiveType& primitiveType, const DrawStates& states)
	{
		if (vertices.size() > BatchingThreshold)
			m_target->draw(vertices, primitiveType, states);

		if (mustFlush(vertices, primitiveType))
			flush();

		assertBufferSize(vertices);

		m_currentPrimitiveType = primitiveType;
		insert(vertices, primitiveType, states);
	}

	void DrawBatch::Buffer::flush() 
	{
		m_target->draw(m_vertices, m_currentPrimitiveType);
		m_vertices.clear();
	}

	inline void DrawBatch::Buffer::assertBufferSize(const std::vector<Vertex>& vertices)
	{
		if (vertices.size() > m_vertices.capacity())
			SB_ERROR() << "The DrawBatch buffer size is too small for the given drawable. Please specify a larger buffer size in the constructor" << std::endl;
	}

	bool DrawBatch::Buffer::mustFlush(const std::vector<Vertex>& vertices, PrimitiveType primitiveType)
	{
		if (m_vertices.empty())
			return false;

		bool bufferTooSmall = m_vertices.size() + vertices.size() > m_vertices.capacity();
		bool primitiveTypeChanged = primitiveType != m_currentPrimitiveType;

		return bufferTooSmall || primitiveTypeChanged;
	}

	void DrawBatch::Buffer::insert(const std::vector<Vertex>& vertices, 
		const PrimitiveType& primitiveType, const DrawStates& states)
	{
		std::vector<Vertex> transformedVertices(vertices);
		transformVertices(transformedVertices, states.transform);

		if (primitiveType == PrimitiveType::Triangles)
			insertTriangles(transformedVertices);
		else if (primitiveType == PrimitiveType::TriangleStrip)
			insertTriangleStrip(transformedVertices);
		else
			SB_ERROR() << "The primitive type " << (int)primitiveType << " is not eligible for batching" << std::endl;
	}

	inline void DrawBatch::Buffer::transformVertices(std::vector<Vertex>& vertices, const Transform& transform)
	{
		for (std::size_t i = 0; i < vertices.size(); i++) 
			vertices[i].position *= transform;
	}

	inline void DrawBatch::Buffer::insertTriangles(const std::vector<Vertex>& vertices)
	{
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	}

	inline void DrawBatch::Buffer::insertTriangleStrip(const std::vector<Vertex>& vertices)
	{
		m_vertices.push_back(vertices[0]);
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
		m_vertices.push_back(vertices[vertices.size() - 1]);
	}
}