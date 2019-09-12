#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb
{
	std::size_t DrawBatch::Buffer::BatchingThreshold = 20;

	void DrawBatch::draw(Drawable& drawable, const DrawState& state)
	{
		m_drawCommands.emplace_back(drawable, state);
	}

	void DrawBatch::draw(DrawTarget& target, DrawState state) 
	{
		m_buffer.setTarget(target);
		 for (std::size_t i = 0; i < m_drawCommands.size(); i++) 
			 m_buffer.draw(m_drawCommands[i].drawable, m_drawCommands[i].state);

		m_buffer.flush();
		m_drawCommands.clear();
	}

	void DrawBatch::Buffer::draw(const std::vector<Vertex>& vertices, 
		const PrimitiveType& primitiveType, const DrawState& state)
	{
		if (vertices.size() > BatchingThreshold)
			m_target->draw(vertices, primitiveType, state);

		if (mustFlush(vertices, primitiveType, state))
			flush();

		assertBufferCapacity(vertices);

		m_currentPrimitiveType = primitiveType;
		m_currentStates = state;
		insert(vertices, primitiveType, state);
	}

	void DrawBatch::Buffer::flush() 
	{
		DrawState state = m_currentStates;
		state.transform = Transform::Identity;
		state.textureTransform = Transform::Identity;
		m_target->draw(m_vertices, m_currentPrimitiveType, state);
		m_vertices.clear();
	}

	inline void DrawBatch::Buffer::assertBufferCapacity(const std::vector<Vertex>& vertices)
	{
		SB_ERROR_IF(vertices.size() > m_vertices.capacity(), 
			"The DrawBatch buffer size is too small for the given drawable. Please specify a larger buffer size in the constructor");
	}

	bool DrawBatch::Buffer::mustFlush(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType, const DrawState& state)
	{
		if (m_vertices.empty())
			return false;

		bool bufferTooSmall = m_vertices.size() + vertices.size() > m_vertices.capacity();
		bool primitiveTypeChanged = primitiveType != m_currentPrimitiveType;
		bool stateChanged = state != m_currentStates;

		return bufferTooSmall || primitiveTypeChanged || stateChanged;
	}

	void DrawBatch::Buffer::insert(const std::vector<Vertex>& vertices, 
		const PrimitiveType& primitiveType, const DrawState& state)
	{
		std::vector<Vertex> transformedVertices(vertices);
		transformVertices(transformedVertices, state);

		if (primitiveType == PrimitiveType::Triangles)
			insertTriangles(transformedVertices);
		else if (primitiveType == PrimitiveType::TriangleStrip)
			insertTriangleStrip(transformedVertices);
		else
			SB_ERROR("The primitive type " << (int)primitiveType << " is not eligible for batching");
	}

	inline void DrawBatch::Buffer::transformVertices(std::vector<Vertex>& vertices, const DrawState& state)
	{
		for (std::size_t i = 0; i < vertices.size(); i++) {
			vertices[i].position *= state.transform;
			vertices[i].texCoords *= state.textureTransform;
		}
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