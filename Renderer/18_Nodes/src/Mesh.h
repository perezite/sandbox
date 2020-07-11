#pragma once

#include "Vertex.h"
#include "PrimitiveType.h"
#include <vector>

namespace sb
{
	class Mesh {
		static bool Locked;
	public:
		Mesh(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType) 
			: m_vertices(vertices), m_primitiveType(primitiveType)
		{ }

		Mesh(const std::size_t numVertices, const PrimitiveType primitiveType)
			: m_vertices(numVertices), m_primitiveType(primitiveType)
		{ }

		Mesh() { }

		virtual ~Mesh() { checkLock(); }

		inline static void lock(bool locked) { Locked = locked; }

		inline static void checkLock() { SB_ERROR_IF(Locked, "Changing a mesh is not allowed while drawing is in progress"); }

		void setVertices(std::vector<Vertex>& vertices) { checkLock(); m_vertices = vertices; }

		void setPrimitiveType(const PrimitiveType primitiveType) { checkLock(); m_primitiveType = primitiveType; }

		std::size_t getVertexCount() const { return m_vertices.size(); }

		inline const std::vector<Vertex>& getVertices() const { return m_vertices; }

		inline std::vector<Vertex>& getVertices() { checkLock(); return m_vertices; }

		inline const PrimitiveType getPrimitiveType() const { return m_primitiveType; }

		Vertex& operator[](std::size_t index) { checkLock(); return m_vertices[index]; }

	private:
		std::vector<Vertex> m_vertices;

		PrimitiveType m_primitiveType;
	};
}

