#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include <vector>
#include <map>

namespace sb
{
	class Renderer
	{
	public:
		Renderer();

		void render(Drawable& drawable, Shader* shader = NULL);

		void display();

	protected:
		void display(std::vector<Drawable*>& drawables, const Material& material);

		void calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result);

		std::size_t getNumVertices(std::vector<Drawable*>& drawables);

		void setupDraw(std::vector<Vertex>& vertices, const Material& material);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(std::vector<Vertex>& vertices);

		void checkGLErrors();

		void cleanupDraw(const Material& material);

	private:
		std::map<Material, std::vector<Drawable*>> m_batches;

		typedef std::map<Material, std::vector<Drawable*>>::iterator BatchIter;

		Shader m_defaultShader;
	};
}