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
		Renderer() : 
			m_numOldDrawables(0), m_numVertices(0), m_numOldIndices(0), m_numIndices(0)
		{ }

		void init();

		void add(Drawable* drawable);

		void remove(Drawable* drawable);

		void render();

	protected:
		void countVertices();

		void countIndices();

		void calcVertices();

		void calcIndices();

		void draw();
		
		void setupDraw();

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void checkGLErrors();

		void cleanupDraw();

	protected:

		static std::size_t accumulateVertices(std::size_t current, sb::Drawable* drawable);

		static std::size_t accumulateIndices(std::size_t current, sb::Drawable* drawable);

	private:
		std::vector<Drawable*> m_drawables;

		std::size_t m_numOldDrawables;

		std::vector<Vertex> m_vertices;

		std::size_t m_numVertices;

		std::vector<GLushort> m_indices;

		std::size_t m_numOldIndices;

		std::size_t m_numIndices;

		Shader m_shader;
	};
}