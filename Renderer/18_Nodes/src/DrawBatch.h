#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "Window.h"
#include "DrawState.h"
#include <map>
#include <tuple>

namespace sb {
	class DrawBatch : public DrawTarget {
		ImmediateDrawTarget& _target;
		std::vector<Vertex> _vertices;
		DrawState _currentState;
		PrimitiveType _currentPrimitiveType;

	protected:
		bool mustFlush(const Mesh& mesh, const DrawState& state);

		void flush();

		void insert(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawState& state);

		inline void transformVertices(std::vector<Vertex>& vertices, const DrawState& state);

		inline void insertTriangles(const std::vector<Vertex>& vertices);

		inline void insertTriangleStrip(const std::vector<Vertex>& vertices);

	public:
		inline DrawBatch(ImmediateDrawTarget& target, size_t capacity = 512)
			: _target(target)
		{
			_vertices.reserve(capacity);
		}
		
		virtual void draw(const Mesh& mesh, const DrawState& state = DrawState::getDefault());

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawState& state = DrawState::getDefault());
	};
}