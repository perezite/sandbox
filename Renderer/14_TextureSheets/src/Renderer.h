#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "PrimitiveType.h"
#include "Transform.h"
#include <vector>

namespace sb
{
	class Renderer
	{
	public:
		void render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states);

		static void resetStatistics();

		static std::size_t getNumDrawCalls();

	protected:
		void setup(const std::vector<Vertex>& vertices, const DrawStates& states);

		Shader* selectShader(const DrawStates& states);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType);

		void cleanup(const DrawStates& states);

	private:

		static std::size_t m_numDrawCalls;
	};
}