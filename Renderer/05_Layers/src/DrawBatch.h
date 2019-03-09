#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "Window.h"
#include <map>
#include <tuple>

namespace sb
{
	class DrawBatch : public DrawTarget
	{
	public:
		DrawBatch(std::size_t bufferCapacity = 512)
			: m_target(NULL), m_unbatchedDrawCalls(0), m_batchedDrawCalls(0)
		{
			m_buffer.reserve(bufferCapacity);
		}

		long getUnbatchedDrawCalls() { return m_unbatchedDrawCalls; }

		long getBatchedDrawCalls() { return m_batchedDrawCalls; }

		void begin(DrawTarget& target);

		void draw(Drawable* drawable, const Transform& transform = Transform::Identity);

		inline void draw(Drawable& drawable, const Transform& transform = Transform::Identity) { draw(&drawable, transform); }

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType = PrimitiveType::Triangles, const Transform& transform = Transform::Identity);

		void end();

	private:
		inline void assertBufferSize(const std::vector<Vertex>& vertices);

		inline bool mustFlush(const std::vector<Vertex>& vertices, PrimitiveType primitiveType);

		inline void flush();

		inline void bufferVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const Transform& transform);

		inline void transformVertices(std::vector<Vertex>& vertices, const Transform& transform);

		inline void bufferTriangles(const std::vector<Vertex>& vertices);

		inline void bufferTriangleStrip(const std::vector<Vertex>& vertices);

	private:
		DrawTarget* m_target;

		std::vector<Vertex> m_buffer;

		PrimitiveType m_currentPrimitiveType;

		long m_unbatchedDrawCalls;

		long m_batchedDrawCalls;
	};
}