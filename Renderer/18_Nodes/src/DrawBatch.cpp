#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb {
	bool DrawBatch::mustFlush(const Mesh& mesh, const DrawState& state)
	{
		if (_vertices.empty())
			return false;
		if (_vertices.size() + mesh.getVertexCount() > _vertices.capacity())
			return true;
		if (mesh.getPrimitiveType() != _currentPrimitiveType)
			return true;
		if (!canBatch(state, _currentState))
			return true;
		return false;
	}

	void DrawBatch::flush() {
		_target.drawImmediate(_vertices, _currentPrimitiveType, _currentState);
		_vertices.clear();
	}

	void DrawBatch::insert(const std::vector<Vertex>& vertices, const PrimitiveType & primitiveType, const DrawState & state) {
		std::vector<Vertex> transformedVertices(vertices);
		transformVertices(transformedVertices, state);

		if (primitiveType == PrimitiveType::Triangles)
			insertTriangles(transformedVertices);
		else if (primitiveType == PrimitiveType::TriangleStrip)
			insertTriangleStrip(transformedVertices);
		else
			SB_ERROR("The primitive type " << (int)primitiveType << " is not eligible for batching");
	}

	inline void DrawBatch::transformVertices(std::vector<Vertex>& vertices, const DrawState& state) {
		for (std::size_t i = 0; i < vertices.size(); i++) {
			vertices[i].position *= state.transform;
			vertices[i].texCoords *= state.textureTransform;
		}
	}

	inline void DrawBatch::insertTriangles(const std::vector<Vertex>& vertices) {
		_vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
	}

	inline void DrawBatch::insertTriangleStrip(const std::vector<Vertex>& vertices) {
		_vertices.push_back(vertices[0]);
		_vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
		_vertices.push_back(vertices[vertices.size() - 1]);
	}

	void DrawBatch::draw(const Mesh& mesh, const DrawState& state) {
		if (mustFlush(mesh, state))
			flush();

		if (mesh.getVertexCount() > _vertices.capacity())
			_target.drawImmediate(mesh.getVertices(), mesh.getPrimitiveType(), state);
		else 
			insert(mesh.getVertices(), mesh.getPrimitiveType(), state);
	}

	void DrawBatch::draw(const std::vector<Vertex>& vertices, const PrimitiveType & primitiveType, const DrawState & state) {
		SB_ERROR("Will be deleted");
	}
}