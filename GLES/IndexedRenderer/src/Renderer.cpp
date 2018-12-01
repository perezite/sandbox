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
		reset();
	}

	void Renderer::add(Drawable* drawable)
	{
		m_drawablesToAdd.push_back(drawable);
		m_numVerticesToAdd += drawable->mesh.getVertexCount();
		m_indexList.add(drawable);
	}

	void Renderer::remove(Drawable* drawable)
	{
		m_drawablesRemove.push_back(drawable);
		m_numVerticesToAdd -= drawable->mesh.getVertexCount();
		m_indexList.remove(drawable);
	}

	void Renderer::render()
	{
		addDrawables();
		removeDrawables();

		m_indexList.calculate();

		resizeVertices();
		calcVertices();

		setupDraw();
		draw();
		cleanupDraw();

		reset();
	}

	void Renderer::addDrawables()
	{
		m_drawables.insert(m_drawables.end(), m_drawablesToAdd.begin(), m_drawablesToAdd.end());
	}

	void Renderer::removeDrawables()
	{
		std::vector<Drawable*>::iterator it = m_drawables.end();
		for (std::size_t i = 0; i < m_drawablesRemove.size(); i++)
			it = std::remove(m_drawables.begin(), it, m_drawablesRemove[i]);

		m_drawables.erase(it, m_drawables.end());
	}

	void Renderer::resizeVertices()
	{
		m_vertices.resize(m_vertices.size() + m_numVerticesToAdd);
	}

	void Renderer::calcVertices()
	{
		std::size_t counter = 0;
		for (std::size_t i = 0; i < m_drawables.size(); i++)
		{
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
		glDrawElements(GL_TRIANGLES, m_indexList.getIndices().size(), GL_UNSIGNED_SHORT, m_indexList.getIndices().data());
		#ifdef _DEBUG
			checkGLErrors();
		#endif
	}

	void Renderer::checkGLErrors()
	{
		GLuint error = glGetError();
		if (error != 0) 
			Error().die() << "GL error: " << error << std::endl;
	}

	void Renderer::cleanupDraw()
	{
		glDisableVertexAttribArray(m_shader.getAttributeLocation("a_vColor"));
		glDisableVertexAttribArray(m_shader.getAttributeLocation("a_vPosition"));
	}

	void Renderer::reset()
	{
		m_drawablesToAdd.clear();
		m_drawablesRemove.clear();
		m_numVerticesToAdd = 0;
	}
}
