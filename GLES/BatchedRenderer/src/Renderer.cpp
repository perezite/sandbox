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

	void Renderer::render()
	{
		addBatches();

		render(&m_mainBatch);

		for (std::size_t i = 0; i < m_batches.size(); i++)
			render(m_batches[i]);
	}

	void Renderer::addBatches()
	{
		m_batches.insert(m_batches.end(), m_batchesToAdd.begin(), m_batchesToAdd.end());
	}

	void Renderer::render(DrawBatch* batch)
	{
		batch->calculate();

		setupDraw(batch);
		draw(batch);
		cleanupDraw();

		reset();
	}

	void Renderer::setupDraw(DrawBatch* batch)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		m_shader.use();
		const std::vector<Vertex>& vertices = batch->getVertices();
		setupVertexAttribPointer(m_shader.getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(vertices[0].position));
		setupVertexAttribPointer(m_shader.getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(vertices[0].color));
	}

	void Renderer::setupVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void Renderer::draw(DrawBatch* batch)
	{
		glDrawElements(GL_TRIANGLES, batch->getIndices().size(), GL_UNSIGNED_SHORT, batch->getIndices().data());
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
		m_batchesToAdd.clear();
	}
}
