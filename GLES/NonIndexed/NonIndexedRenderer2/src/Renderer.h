#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include <vector>
#include <map>

namespace sb
{
	//struct DrawConfig {
	//	DrawConfig(Drawable* drawable_, Shader* shader_)
	//		: drawable(drawable_), shader(shader_)
	//	{ }

	//	Drawable* drawable;
	//	Shader* shader;
	//};

	class Renderer
	{
	public:
		void init();

		void render(Drawable& drawable, Shader* shader = NULL);

		void display();

	protected:
		void display(std::vector<Drawable*>& drawables, Shader* shader);

		void calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result);

		std::size_t getNumVertices(std::vector<Drawable*>& drawables);

		void setupDraw(std::vector<Vertex>& vertices, Shader* shader);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(std::vector<Vertex>& vertices);

		void checkGLErrors();

		void cleanupDraw(Shader* shader);

	private:
		std::map<Shader*, std::vector<Drawable*>> m_batches;
		typedef std::map<Shader*, std::vector<Drawable*>>::iterator BatchIter;

		// std::vector<Vertex> m_vertices;

		Shader m_defaultShader;
	};
}