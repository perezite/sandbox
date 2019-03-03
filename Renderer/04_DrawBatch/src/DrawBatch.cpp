#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"

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
	}

	void DrawBatch::drawShapes(const std::vector<Shape*>& shapes, const Substance& substance, Window& window, const Transform& transform)
	{
		PrimitiveType primitiveType = substance.primitiveType;

		if (primitiveType == PrimitiveType::Triangles)
			drawTriangleShapes(shapes, window, transform);
	}

	void DrawBatch::drawTriangleShapes(const std::vector<Shape*>& shapes, Window& window, const Transform& transform)
	{
		m_buffer.clear();

		for (std::size_t i = 0; i < shapes.size(); i++) {
			SB_ERROR_IF(shapes[i]->getMesh().getVertexCount() > m_buffer.capacity()) << "The vertex count of the given shape exceeds the draw batch capacity" << std::endl;

			bufferTriangleShape(shapes[i], transform);
		}
	}

	void DrawBatch::bufferTriangleShape(const Shape* shape, const Transform& transform) {
		// Mesh transformedMesh = transform * shape->getMesh();
		// Mesh transformedMesh = transform * mesh;
	}
}
