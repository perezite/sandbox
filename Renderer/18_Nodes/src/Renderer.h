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
		void render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& state);

		static std::size_t getNumDrawCalls();
		
		static void resetStatistics();

	protected:
		Shader* selectShader(const DrawStates& state);
		
		void setup(Shader* shader, const std::vector<Vertex>& vertices, const DrawStates& state);

		void setupShaderUniforms(Shader* shader, const DrawStates& state);

		void setupShaderAttributes(Shader* shader, const std::vector<Vertex>& vertices, const DrawStates& state);

		void setShaderAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType);

		void cleanup(Shader* shader, const DrawStates& state);

	private:
		static std::size_t m_numDrawCalls;
	};
}