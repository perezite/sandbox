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
		void init();

		void add(Drawable& drawable);

		void remove(Drawable& drawable);

		void render();

	protected:
		void calcVertices();

		std::size_t getNumVertices();

		void draw();

		void setupDraw();

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void checkGLErrors();

		void cleanupDraw();

	private:
		std::vector<Drawable*> m_drawables;

		std::vector<Vertex> m_vertices;

		Shader m_shader;
	};
}