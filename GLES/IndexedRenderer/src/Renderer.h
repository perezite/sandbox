#pragma once

#include "GL.h"
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

		void add(Drawable* drawable);

		void remove(Drawable* drawable);

		void render();

	protected:
		void calcVertices();

		std::size_t countVertices();

		void calcIndices();

		std::size_t countIndices();

		void draw();

		void setupDraw();

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void checkGLErrors();

		void cleanupDraw();

	private:
		std::vector<Drawable*> m_drawables;

		std::vector<Vertex> m_vertices;

		std::vector<GLushort> m_indices;

		bool m_drawablesChanged;

		Shader m_shader;

	};
}