#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include <vector>
#include <map>

namespace sb
{	
	class Renderer
	{
	public:
		void render(Drawable* drawable);

		void display();

	protected:
		void display(std::vector<Drawable*>& drawables);

		void display(std::vector<Vertex>& vertices, std::vector<GLushort>& indices);

		void calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result);

		std::size_t getNumVertices(std::vector<Drawable*>& drawables);

		void calcIndices(std::vector<Drawable*>& drawables, std::vector<GLushort>& result);

		std::size_t getNumIndices(std::vector<Drawable*>& drawables);

		void setupDraw(std::vector<Vertex>& vertices);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(std::vector<GLushort>& indices);

		void checkGLErrors();

		void cleanupDraw();

	private:
		std::vector<Drawable*> m_batch;
	};
}