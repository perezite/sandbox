#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include <vector>

namespace sb
{
	class Renderer
	{
	public:
		void render(const std::vector<Vertex>& vertices);

	protected:
		void setup(const std::vector<Vertex>& vertices);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(const std::vector<Vertex>& vertices);

		void cleanup();

		void checkGLErrors();

	private:
		Shader m_shader;
	};
}