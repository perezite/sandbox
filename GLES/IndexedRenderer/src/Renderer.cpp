#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>

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
		if (m_drawables.size() > m_numOldDrawables) {
			countVertices();
			countIndices();
			calcIndices();
		}

		calcVertices();

		setupDraw();
		draw();
		cleanupDraw();

		m_numOldDrawables = m_drawables.size();
	}

	void Renderer::countIndices()
	{
		m_numOldIndices = m_numIndices;
		m_numIndices = std::accumulate(m_drawables.begin() + m_numOldDrawables, m_drawables.end(), m_numIndices, &Renderer::accumulateIndices);
	}

	void Renderer::countVertices()
	{
		m_numVertices = std::accumulate(m_drawables.begin() + m_numOldDrawables, m_drawables.end(), m_numVertices, &Renderer::accumulateVertices);
		Error().dieIf(m_numVertices > 65536) << "There are more than 65536 vertices in a single draw batch, aborting" << std::endl;
	}

	void Renderer::calcIndices()
	{
		m_indices.resize(m_numIndices);

		unsigned int offset = m_numOldIndices;
		unsigned int counter = 0;
		for (std::size_t i = m_numOldDrawables; i < m_drawables.size(); i++) {
			const std::vector<GLushort>& indices = m_drawables[i]->mesh.getIndices();
			for (std::size_t j = 0; j < indices.size(); j++)
				m_indices[counter + j] = indices[j] + offset;
			offset += m_drawables[i]->mesh.getVertexCount();
			counter += indices.size();
		}
	}

	void Renderer::calcVertices()
	{
		m_vertices.resize(m_numVertices);
	
		unsigned int counter = 0;
		for (std::size_t i = 0; i < m_drawables.size(); i++) {
			for (std::size_t j = 0; j < m_drawables[i]->mesh.getVertexCount(); j++) {
				m_vertices[counter].position = m_drawables[i]->transform * m_drawables[i]->mesh[j].position;		
				m_vertices[counter].color = m_drawables[i]->mesh[j].color;
				counter++;
			}
		}
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
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, m_indices.data());
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

	std::size_t Renderer::accumulateVertices(std::size_t current, sb::Drawable* drawable)
	{
		return current + drawable->mesh.getVertexCount();
	}

	std::size_t Renderer::accumulateIndices(std::size_t current, sb::Drawable* drawable)
	{
		return current + drawable->mesh.getIndexCount();
	}
}