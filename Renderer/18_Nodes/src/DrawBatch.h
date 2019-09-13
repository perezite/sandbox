#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "Window.h"
#include "DrawStates.h"
#include <map>
#include <tuple>

namespace sb {
	class DrawBatch : public DrawTarget {
		ImmediateDrawTarget& _target;
		std::vector<Vertex> _vertices;
		DrawStates _currentState;
		PrimitiveType _currentPrimitiveType;

	protected:
		bool mustFlush(const Mesh& mesh, const DrawStates& state);

		void flush();

		void insert(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& state);

		inline void transformVertices(std::vector<Vertex>& vertices, const DrawStates& state);

		inline void insertTriangles(const std::vector<Vertex>& vertices);

		inline void insertTriangleStrip(const std::vector<Vertex>& vertices);

	public:
		inline DrawBatch(ImmediateDrawTarget& target, size_t capacity = 512)
			: _target(target)
		{
			_vertices.reserve(capacity);
		}
		
		void setTarget(ImmediateDrawTarget& target);

		virtual void draw(const Mesh& mesh, const DrawStates& state = DrawStates::getDefault());

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& state = DrawStates::getDefault());

		void complete();
	};
}