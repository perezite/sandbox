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
		void render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawState& state);

		static std::size_t getNumDrawCalls();
		
		static void resetStatistics();

	protected:
		Shader* selectShader(const DrawState& state);
		
		void setup(Shader* shader, const std::vector<Vertex>& vertices, const DrawState& state);

		void setupShaderUniforms(Shader* shader, const DrawState& state);

		void setupShaderAttributes(Shader* shader, const std::vector<Vertex>& vertices, const DrawState& state);

		void setShaderAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType);

		void cleanup(Shader* shader, const DrawState& state);

	private:
		static std::size_t m_numDrawCalls;
	};
}