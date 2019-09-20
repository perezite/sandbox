#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "Window.h"
#include "DrawStates.h"
#include <map>
#include <tuple>

namespace sb {
	class DrawBatch : public DrawTarget {
		ImmediateDrawTarget* _target;
		std::vector<Vertex> _vertices;
		DrawStates _currentStates;
		PrimitiveType _currentPrimitiveType;

	protected:
		bool mustFlush(const Mesh& mesh, const DrawStates& states);

		void flush();

		void insert(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states);

		inline void transformVertices(std::vector<Vertex>& vertices, const DrawStates& states);

		inline void insertTriangles(const std::vector<Vertex>& vertices);

		inline void insertTriangleStrip(const std::vector<Vertex>& vertices);

		inline void drawImmediate(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states);

	public:
		inline DrawBatch(ImmediateDrawTarget* target = NULL, size_t capacity = 512)
			: _target(target)
		{
			_vertices.reserve(capacity);
		}
		
		void setTarget(ImmediateDrawTarget& target);

		virtual void draw(const Mesh& mesh, const DrawStates& states = DrawStates::getDefault());

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& states = DrawStates::getDefault());

		void complete();
	};
}