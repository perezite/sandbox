#pragma once

#include "Drawable.h"
#include "DrawBatch.h"
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
		void render(Drawable& drawable);

		void render(DrawBatch& batch);

		void display();

	protected:
		void display(std::vector<Drawable*>& drawables, const Material& material);

		void calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result);

		std::size_t getNumVertices(std::vector<Drawable*>& drawables);

		void calcIndices(std::vector<Drawable*>& drawables, std::vector<GLushort>& result);

		std::size_t getNumIndices(std::vector<Drawable*>& drawables);

		void setupDraw(std::vector<Vertex>& vertices, const Material& material);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(std::vector<GLushort>& indices);

		void checkGLErrors();

		void cleanupDraw(const Material& material);

	private:
		std::map<Material, std::vector<Drawable*>> m_dynamicBatches;

		typedef std::map<Material, std::vector<Drawable*>>::iterator BatchIter;

		std::vector<DrawBatch*> m_batches;
	};
}