#pragma once
#include "Drawable.h"
#include "Shape.h"
#include "Substance.h"
#include <map>
#include <tuple>

namespace sb
{
	class DrawBatch : public Drawable 
	{
	public:
		DrawBatch(std::size_t maxNumBufferedVertices = 512)
			: m_buffer(maxNumBufferedVertices)
		{ }

		virtual void draw(Window& window, Transform transform);

		void draw(Shape& shape);

		void draw(Shape* shape);

	protected:
		void drawShapes(std::vector<Shape*>& shapes, const Substance& substance, Window& window, const Transform& transform);

		inline void insertShape(Shape* shape, PrimitiveType primitiveType);

		inline void insertTriangles(const std::vector<Vertex>& vertices);

		inline void insertTriangleStrip(const std::vector<Vertex>& vertices);

		inline void flush(Window& window, const PrimitiveType primitiveType, const Transform& transform);

		inline bool bufferHasCapacity(Shape* shape);

	private: 
		typedef std::map<Substance, std::vector<Shape*>> DrawCallMap;
		DrawCallMap m_drawCalls;

		std::vector<Vertex> m_buffer;
	};
}