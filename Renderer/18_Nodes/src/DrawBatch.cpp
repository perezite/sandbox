#include "DrawBatch.h"
#include "Window.h"
#include "Logger.h"
#include "Transform.h"

namespace sb {
	bool DrawBatch::mustFlush(const Mesh& mesh, const DrawStates& states)
	{
		if (_vertices.empty())
			return false;
		if (_vertices.size() + mesh.getVertexCount() > _vertices.capacity())
			return true;
		if (mesh.getPrimitiveType() != _currentPrimitiveType)
			return true;
		if (!canBatch(states, _currentState))
			return true;
		return false;
	}

	void DrawBatch::flush() {
		drawImmediate(_vertices, _currentPrimitiveType, _currentState);
		_vertices.clear();
	}

	void DrawBatch::insert(const std::vector<Vertex>& vertices, const PrimitiveType & primitiveType, const DrawStates & states) {
		std::vector<Vertex> transformedVertices(vertices);
		transformVertices(transformedVertices, states);

		if (primitiveType == PrimitiveType::Triangles)
			insertTriangles(transformedVertices);
		else if (primitiveType == PrimitiveType::TriangleStrip)
			insertTriangleStrip(transformedVertices);
		else
			SB_ERROR("The primitive type " << (int)primitiveType << " is not eligible for batching");
	}

	inline void DrawBatch::transformVertices(std::vector<Vertex>& vertices, const DrawStates& states) {
		for (std::size_t i = 0; i < vertices.size(); i++) {
			vertices[i].position *= states.transform;
			vertices[i].texCoords *= states.textureTransform;
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

	inline void DrawBatch::drawImmediate(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states) {
		SB_ERROR_IF(_target == NULL, "No target was set for the draw batch");
		_target->drawImmediate(vertices, primitiveType, states);
	}

	void DrawBatch::setTarget(ImmediateDrawTarget& target) {
		complete();
		_target = &target;
	}

	void DrawBatch::draw(const Mesh& mesh, const DrawStates& states) {
		if (mustFlush(mesh, states))
			flush();

		if (mesh.getVertexCount() > _vertices.capacity())
			drawImmediate(mesh.getVertices(), mesh.getPrimitiveType(), states);
		else {
			_currentPrimitiveType = mesh.getPrimitiveType();
			insert(mesh.getVertices(), mesh.getPrimitiveType(), states);
		}
	}

	void DrawBatch::draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states) {
		SB_ERROR("Will be deleted");
	}

	void DrawBatch::complete() {
		if (!_vertices.empty())
			flush();
	}
}