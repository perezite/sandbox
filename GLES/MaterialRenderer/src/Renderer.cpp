#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>

namespace sb
{
	void Renderer::add(Drawable* drawable)
	{
		for (std::size_t i = 0; i < m_generatedBatches.size(); i++)
			if (m_generatedBatches[i].getMaterial() == drawable->material) {
				m_generatedBatches[i].add(drawable);
				return;
			}

		generateBatch(drawable);
	}

	void Renderer::add(Drawable* drawable) 
	{

	}

	void Renderer::remove(Drawable* drawable)
	{
		for (std::size_t i = 0; i < m_generatedBatches.size(); i++)
			if (m_generatedBatches[i].getMaterial() == drawable->material) 
				m_generatedBatches[i].remove(drawable);
	}
	
	void Renderer::draw()
	{
		cleanupGeneratedBatches();
		for (std::size_t i = 0; i < m_generatedBatches.size(); i++)
			draw(&m_generatedBatches[i]);

		addNewBatches();
		for (std::size_t i = 0; i < m_batches.size(); i++)
			draw(m_batches[i]);

		reset();
	}

	void Renderer::cleanupGeneratedBatches()
	{
		for (std::size_t i = 0; i < m_generatedBatches.size(); i++) 
			if (m_generatedBatches[i].getDrawableCount() == 0) 
				m_generatedBatches.erase(m_generatedBatches.begin() + i);
	}

	// TODO: Add batch deletion
	void Renderer::addNewBatches()
	{
		m_batches.insert(m_batches.end(), m_batchesToAdd.begin(), m_batchesToAdd.end());
	}

	void Renderer::draw(DrawBatch* batch)
	{
		batch->calculate();

		setupDraw(batch);
		drawBatch(batch);
		cleanupDraw(batch);
	}

	void Renderer::setupDraw(DrawBatch* batch)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		const std::vector<Vertex>& vertices = batch->getVertices();
		Shader* shader = batch->getMaterial().getShader();
		shader->use();
		// TODO: Move to shader
		setupVertexAttribPointer(shader->getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(vertices[0].position));
		setupVertexAttribPointer(shader->getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(vertices[0].color));
	}

	void Renderer::setupVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void Renderer::drawBatch(DrawBatch* batch)
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

	void Renderer::cleanupDraw(DrawBatch* batch)
	{
		Shader* shader = batch->getMaterial().getShader();
		glDisableVertexAttribArray(shader->getAttributeLocation("a_vColor"));
		glDisableVertexAttribArray(shader->getAttributeLocation("a_vPosition"));
	}

	void Renderer::reset()
	{
		m_batchesToAdd.clear();
	}
}
