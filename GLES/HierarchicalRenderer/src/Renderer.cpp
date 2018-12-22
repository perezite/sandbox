#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	void Renderer::render(Drawable* drawable)
	{
		m_layers[drawable->getLayer()].dynamicBatches[drawable->getMaterial()].push_back(drawable);
	}

	void Renderer::render(DrawBatch& drawBatch)
	{
		m_layers[drawBatch.getLayer()].drawBatches.push_back(&drawBatch);
	}

	void Renderer::display() 
	{
		for (std::map<int, Layer>::iterator it = m_layers.begin(); it != m_layers.end(); it++) {
			display(it->second.dynamicBatches, it->second.drawBatches);
		}

		m_layers.clear();
	}

	void Renderer::display(DynamicBatchMap& dynamicBatches, std::vector<DrawBatch*> drawBatches)
	{
		for (std::size_t i = 0; i < drawBatches.size(); i++)
			display(drawBatches[i]);

		for (DynamicBatchMap::iterator it = dynamicBatches.begin(); it != dynamicBatches.end(); it++)
			display(it->second, it->first);
	}

	void Renderer::display(DrawBatch* drawBatch)
	{
		std::vector<Vertex> vertices;
		drawBatch->calcVertices(vertices);

		display(vertices, drawBatch->getIndices(), drawBatch->getMaterial());
	}

	void Renderer::display(std::vector<Drawable*>& drawables, const Material& material)
	{
		std::vector<Vertex> vertices;
		calcVertices(drawables, vertices);

		std::vector<GLushort> indices;
		calcIndices(drawables, indices);

		display(vertices, indices, material);
	}

	void Renderer::display(std::vector<Vertex>& vertices, std::vector<GLushort>& indices, const Material& material)
	{
		setupDraw(vertices, material);
		draw(indices);
		cleanupDraw(material);
	}

	void Renderer::calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result)
	{
		result.resize(getNumVertices(drawables));
		
		unsigned int count = 0;
		for (std::size_t i = 0; i < drawables.size(); i++) {
			for (std::size_t j = 0; j < drawables[i]->getMesh().getVertexCount(); j++) {
				result[count].position = drawables[i]->getTransform() * drawables[i]->getMesh()[j].position;
				result[count].color = drawables[i]->getMesh()[j].color;
				count++;
			}
		}
	}

	std::size_t Renderer::getNumVertices(std::vector<Drawable*>& drawables)
	{
		std::size_t count = 0;
		for (std::size_t i = 0; i < drawables.size(); i++)
			count += drawables[i]->getMesh().getVertexCount();

		return count;
	}

	void Renderer::calcIndices(std::vector<Drawable*>& drawables, std::vector<GLushort>& result)
	{
		result.resize(getNumIndices(drawables));
		std::size_t count = 0;
		GLushort offset = 0;

		for (std::size_t i = 0; i < drawables.size(); i++) {
			const std::vector<GLushort>& indices = drawables[i]->getMesh().getIndices();
			std::copy(indices.begin(), indices.end(), result.begin() + count);
			for (std::size_t j = 0; j < indices.size(); j++)
				result[count + j] = indices[j] + offset;

			count += indices.size();
			offset += (GLushort)drawables[i]->getMesh().getVertexCount();
		}
	}

	std::size_t Renderer::getNumIndices(std::vector<Drawable*>& drawables)
	{
		std::size_t count = 0;

		for (std::size_t i = 0; i < drawables.size(); i++)
			count += drawables[i]->getMesh().getIndexCount();

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