#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb
{
	void DrawBatch::draw(Shape& shape)
	{
		m_drawCalls[Substance(shape)].push_back(&shape);
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
		m_buffer.clear();

		PrimitiveType primitiveType = substance.primitiveType;
		if (primitiveType == PrimitiveType::Triangles)
			drawTriangleShapes(shapes, window, transform);
		else
			SB_ERROR() << "The PrimitiveType " << (int)primitiveType << " is not eligible for batching" << std::endl;

		window.draw(m_buffer, primitiveType, transform);
	}

	void DrawBatch::drawTriangleShapes(std::vector<Shape*>& shapes, Window& window, const Transform& transform)
	{
		for (std::size_t i = 0; i < shapes.size(); i++) {
			SB_ERROR_IF(shapes[i]->getMesh().getVertexCount() > m_buffer.capacity()) 
				<< "The vertex count of the given shape exceeds the draw batch capacity" << std::endl;
			bufferTriangleShape(shapes[i]);
		}

	}

	void DrawBatch::bufferTriangleShape(Shape* shape) {
		Mesh transformedMesh = shape->getTransform() * shape->getMesh();
		std::vector<Vertex> vertices = transformedMesh.getVertices();
		m_buffer.insert(m_buffer.end(), vertices.begin(), vertices.end());
	}
}
