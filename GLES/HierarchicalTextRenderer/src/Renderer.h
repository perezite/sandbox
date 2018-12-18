#pragma once

#include "Mesh.h"
#include "Vertex.h"
#include "Shader.h"
#include <vector>
#include <map>

namespace sb
{	
	class Renderer
	{
	public:
		void render(Mesh& mesh);

		void display();

	protected:

		void display(std::vector<Vertex>& vertices, std::vector<GLushort>& indices);

		void calcVertices(std::vector<Mesh*>& meshes, std::vector<Vertex>& result);

		std::size_t getNumVertices(std::vector<Mesh*>& meshes);

		void calcIndices(std::vector<Mesh*>& meshes, std::vector<GLushort>& result);

		std::size_t getNumIndices(std::vector<Mesh*>& meshes);

		void setupDraw(std::vector<Vertex>& vertices);

		void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(std::vector<GLushort>& indices);

		void checkGLErrors();

		void cleanupDraw();

	private:
		std::vector<Mesh*> m_batch;
	};
}