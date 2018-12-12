#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	void Renderer::render(Drawable& drawable)
	{
		m_dynamicBatches[drawable.material].push_back(&drawable);
	}

	void Renderer::render(DrawBatch& batch)
	{
		m_batches.push_back(&batch);
	}

	void Renderer::display()
	{
		for (BatchIter it = m_dynamicBatches.begin(); it != m_dynamicBatches.end(); it++)
			display(it->second, it->first);

		//for (std::size_t i = 0; i < m_batches.size(); i++)
		//	display(m_batches[i]->getDrawables(), m_batches[i]->getMaterial());

		m_batches.clear();
		m_dynamicBatches.clear();
	}

	void Renderer::display(std::vector<Drawable*>& drawables, const Material& material)
	{
		std::vector<Vertex> vertices;
		calcVertices(drawables, vertices);

		std::vector<GLushort> indices;
		calcIndices(drawables, indices);

		setupDraw(vertices, material);
		draw(indices);
		cleanupDraw(material);
	}

	void Renderer::calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result)
	{
		result.resize(getNumVertices(drawables));
		
		unsigned int count = 0;
		for (std::size_t i = 0; i < drawables.size(); i++) {
			for (std::size_t j = 0; j < drawables[i]->mesh.getVertexCount(); j++) {
				result[count].position = drawables[i]->transform * drawables[i]->mesh[j].position;
				result[count].color = drawables[i]->mesh[j].color;
				count++;
			}
		}
	}

	std::size_t Renderer::getNumVertices(std::vector<Drawable*>& drawables)
	{
		std::size_t count = 0;
		for (std::size_t i = 0; i < drawables.size(); i++)
			count += drawables[i]->mesh.getVertexCount();

		return count;
	}

	void Renderer::calcIndices(std::vector<Drawable*>& drawables, std::vector<GLushort>& result)
	{
		result.resize(getNumIndices(drawables));

		std::size_t position = 0;
		GLushort offset = 0;
		for (std::size_t i = 0; i < drawables.size(); i++) {
			const std::vector<GLushort>& indices = drawables[i]->mesh.getIndices();
			result.insert(result.end(), indices.begin(), indices.end());
			for (std::size_t j = 0; j < indices.size(); j++)
				result[position + j] = indices[j] + offset;

			position += indices.size();
			offset += (GLushort)drawables[i]->mesh.getVertexCount();
		}
	}

	std::size_t Renderer::getNumIndices(std::vector<Drawable*>& drawables)
	{
		std::size_t count = 0;

		for (std::size_t i = 0; i < drawables.size(); i++)
			count += drawables[i]->mesh.getIndexCount();

		return count;
	}

	void Renderer::setupDraw(std::vector<Vertex>& vertices, const Material& material)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		material.shader->use();
		setVertexAttribPointer(material.shader->getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(vertices[0].position));
		setVertexAttribPointer(material.shader->getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(vertices[0].color));
	}

	void Renderer::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void Renderer::draw(std::vector<GLushort>& indices)
	{
		// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());
		checkGLErrors();
	}

	void Renderer::checkGLErrors()
	{
		GLuint error = glGetError();
		if (error != 0) {
			Error().die() << "GL error: " << error << std::endl;
		}
	}

	void Renderer::cleanupDraw(const Material& material)
	{
		glDisableVertexAttribArray(material.shader->getAttributeLocation("a_vColor"));
		glDisableVertexAttribArray(material.shader->getAttributeLocation("a_vPosition"));
	}
}