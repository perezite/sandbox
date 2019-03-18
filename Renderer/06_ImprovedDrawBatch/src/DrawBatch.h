#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "Window.h"
#include <map>
#include <tuple>

namespace sb
{
	class DrawBatch : public Drawable
	{
	private:
		struct DrawCommand {
			DrawCommand(Drawable& drawable_, Transform transform_)
				: drawable(drawable_), transform(transform_)
			{ }

			Drawable& drawable;
			Transform transform;
		};

		class Buffer : public DrawTarget {
		public:
			Buffer(std::size_t capacity)
			{
				m_vertices.reserve(capacity);
			}

			inline void setTarget(DrawTarget& target) { m_target = &target; }

			void draw(Drawable& drawable, Transform& transform);

			virtual void draw(const std::vector<Vertex>& vertices,
				const PrimitiveType& primitiveType, const Transform& transform = Transform::Identity);

			void flush();

		protected:
			void assertBufferSize(const std::vector<Vertex>& vertices);

			bool mustFlush(const std::vector<Vertex>& vertices, PrimitiveType primitiveType);

			void insert(const std::vector<Vertex>& vertices, 
				const PrimitiveType& primitiveType, const Transform& transform);

			inline void transformVertices(std::vector<Vertex>& vertices, const Transform& transform);

			inline void insertTriangles(const std::vector<Vertex>& vertices);

			inline void insertTriangleStrip(const std::vector<Vertex>& vertices);

		private:
			static std::size_t BatchingThreshold;

			DrawTarget* m_target;

			std::vector<Vertex> m_vertices;

			PrimitiveType m_currentPrimitiveType;
		};

	public:
		DrawBatch(std::size_t bufferCapacity = 512)
			: m_buffer(bufferCapacity)
		{
			m_drawCommands.reserve(bufferCapacity / 4);
		}

		void draw(Drawable& drawable, const Transform& transform = Transform::Identity);

		virtual void draw(DrawTarget& target, Transform transform);

	private:
		Buffer m_buffer;

		std::vector<DrawCommand> m_drawCommands;
	};
}