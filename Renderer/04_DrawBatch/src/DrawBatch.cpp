#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb
{
	inline void DrawBatch::draw(Shape& shape)
	{
		draw(&shape);
	}

	void DrawBatch::draw(Shape* shape)
	{
		m_drawCalls[Substance(shape)].push_back(shape);
	}

	void DrawBatch::draw(Window& window, Transform transform)
	{
		DrawCallMap::iterator drawCall;
		for (drawCall = m_drawCalls.begin(); drawCall != m_drawCalls.end(); drawCall++) 
			drawShapes(drawCall->second, drawCall->first, window, transform);

		m_drawCalls.clear();
	}

	void DrawBatch::drawShapes(std::vector<Shape*>& shapes, const Substance& substance, Window& window, const Transform& transform)
	{
		PrimitiveType primitiveType = substance.primitiveType;
		m_buffer.clear();

		for (std::size_t i = 0; i < shapes.size(); i++) {
			if (!bufferHasCapacity(shapes[i]))
				flush(window, primitiveType, transform);

			insertShape(shapes[i], primitiveType);
		}

		if (m_buffer.size() > 0)
			flush(window, primitiveType, transform);
	}

	inline void DrawBatch::insertShape(Shape* shape, PrimitiveType primitiveType) 
	{
		Mesh transformedMesh = shape->getTransform() * shape->getMesh();
		const std::vector<Vertex>& vertices = transformedMesh.getVertices();

		if (primitiveType == PrimitiveType::Triangles)
			insertTriangles(vertices);
		else if (primitiveType == PrimitiveType::TriangleStrip)
			insertTriangleStrip(vertices);
		else
			SB_ERROR() << "The primitive type " << (int)primitiveType << "is not eligible for batching" << std::endl;
	}

	inline void DrawBatch::insertTriangles(const std::vector<Vertex>& vertices) 
	{
		m_buffer.insert(m_buffer.end(), vertices.begin(), vertices.end());
	}

	inline void DrawBatch::insertTriangleStrip(const std::vector<Vertex>& vertices) 
	{
		m_buffer.push_back(vertices[0]);
		m_buffer.insert(m_buffer.end(), vertices.begin(), vertices.end());
		m_buffer.push_back(vertices[vertices.size() - 1]);
	}

	inline void DrawBatch::flush(Window& window, const PrimitiveType primitiveType, const Transform& transform)
	{
		window.draw(m_buffer, primitiveType, transform);
		m_buffer.clear();
	}

	inline bool DrawBatch::bufferHasCapacity(Shape* shape)
	{
		if (shape->getMesh().getVertexCount() > m_buffer.capacity())
			SB_ERROR() << "The vertex count of the given shape exceeds the draw batch capacity" << std::endl;
			
		return m_buffer.size() + shape->getMesh().getVertexCount() <= m_buffer.capacity();
	}

}
