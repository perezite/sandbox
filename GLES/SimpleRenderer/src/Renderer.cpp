#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	void Renderer::init()
	{
		m_shader.init();
	}

	void Renderer::add(Drawable* drawable)
	{
		if (std::find(m_drawables.begin(), m_drawables.end(), drawable) != m_drawables.end())
			return;

		m_drawables.push_back(drawable);
	}

	void Renderer::remove(Drawable* drawable)
	{
		m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), drawable), m_drawables.end());
	}

	void Renderer::render()
	{
		calcVertices();
		setupDraw();
		draw();
		cleanupDraw();
	}

	void Renderer::calcVertices()
	{
		m_vertices.resize(getNumVertices());
		
		unsigned int counter = 0;
		for (std::size_t i = 0; i < m_drawables.size(); i++) {
			for (std::size_t j = 0; j < m_drawables[i]->mesh.getVertexCount(); j++) {
				m_vertices[counter].position = m_drawables[i]->transform * m_drawables[i]->mesh[j].position;
				m_vertices[counter].color = m_drawables[i]->mesh[j].color;
				counter++;
			}
		}
	}

	std::size_t Renderer::getNumVertices()
	{
		std::size_t numVertices = 0;
		for (std::size_t i = 0; i < m_drawables.size(); i++)
			numVertices += m_drawables[i]->mesh.getVertexCount();

		return numVertices;
	}


	void Renderer::setupDraw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		m_shader.use();
		setVertexAttribPointer(m_shader.getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(m_vertices[0].position));
		setVertexAttribPointer(m_shader.getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(m_vertices[0].color));
	}

	void Renderer::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void Renderer::draw()
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		checkGLErrors();
	}

	void Renderer::checkGLErrors()
	{
		GLuint error = glGetError();
		if (error != 0) {
			Error().die() << "GL error: " << error << std::endl;
		}
	}

	void Renderer::cleanupDraw()
	{
		glDisableVertexAttribArray(m_shader.getAttributeLocation("a_vColor"));
		glDisableVertexAttribArray(m_shader.getAttributeLocation("a_vPosition"));
	}
}