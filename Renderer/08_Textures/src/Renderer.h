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
		Renderer()
			: m_shader(Shader::getDefault())
		{ }

		void render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states);

		static void resetStatistics();

		static std::size_t getNumDrawCalls();

	protected:
		void setup(const std::vector<Vertex>& vertices, const DrawStates& states);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType);

		void cleanup();

		void checkGLErrors();

	private:
		Shader& m_shader;

		static std::size_t m_numDrawCalls;
	};
}