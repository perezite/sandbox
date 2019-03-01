#include "Renderer.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	void Renderer::render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType)
	{
		if (vertices.empty())
			return;

		setup(vertices);
		draw(vertices, primitiveType);
		cleanup();
	}

	void Renderer::setup(const std::vector<Vertex>& vertices)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		m_shader.use();

		GLvoid* positionStart = (GLvoid*) &(vertices[0].position);
		GLvoid* colorStart = (GLvoid*) &(vertices[0].color);
		setVertexAttribPointer(m_shader.getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), positionStart);
		setVertexAttribPointer(m_shader.getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), colorStart);
	}

	void Renderer::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void Renderer::draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType)
	{
		glDrawArrays((GLenum)primitiveType, 0, vertices.size());
		checkGLErrors();
	}

	void Renderer::checkGLErrors()
	{
		GLuint glError = glGetError();
		SB_ERROR_IF(glError != 0) << "GL error: " << glError << std::endl;
	}

	void Renderer::cleanup()
	{
		glDisableVertexAttribArray(m_shader.getAttributeLocation("a_vColor"));
		glDisableVertexAttribArray(m_shader.getAttributeLocation("a_vPosition"));
	}
}