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

	protected:
		void drawShapes(const std::vector<Shape*>& shapes, const Substance& substance, Window& window, const Transform& transform);

		void drawTriangleShapes(const std::vector<Shape*>& shapes, Window& window, const Transform& transform);

		void bufferTriangleShape(const Shape* shape, const Transform& transform);

	private: 
		typedef std::map<Substance, std::vector<Shape*>> DrawCallMap;
		DrawCallMap m_drawCalls;

		std::vector<Vertex> m_buffer;
	};
}