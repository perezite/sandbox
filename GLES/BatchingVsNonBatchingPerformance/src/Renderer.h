#pragma once
#include "GL.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexList.h" 

#define ORB_PLATFORM_SPECIFIC() namespace { char dummy; }

 #define ORB_USE_DEFAULT_RENDERER
//#define ORB_USE_NAIVE_RENDERER

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
		void addDrawables();

		void removeDrawables();

		void resizeVertices();

		void calcVertices();

		void setupDraw();

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw();

		void checkGLErrors();

		void cleanupDraw();

		void reset();

	private:
		std::vector<Drawable*> m_drawables;

		std::vector<Vertex> m_vertices;

		IndexList m_indexList;

		std::vector<Drawable*> m_drawablesToAdd;

		std::vector<Drawable*> m_drawablesRemove;

		std::size_t m_numVerticesToAdd;

		Shader m_shader;

	};
}
