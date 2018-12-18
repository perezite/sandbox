#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	void Renderer::render(Mesh& mesh)
	{
		m_batch.push_back(&mesh);
	}

	void Renderer::display()
	{
		std::vector<Vertex> vertices;
		calcVertices(m_batch, vertices);

		std::vector<GLushort> indices;
		calcIndices(m_batch, indices);

		display(vertices, indices);
	}

	void Renderer::display(std::vector<Vertex>& vertices, std::vector<GLushort>& indices)
	{
		setupDraw(vertices);
		draw(indices);
		cleanupDraw();
	}

	void Renderer::calcVertices(std::vector<Mesh*>& meshes, std::vector<Vertex>& result)
	{
		result.resize(getNumVertices(meshes));
		
		unsigned int count = 0;
		for (std::size_t i = 0; i < meshes.size(); i++) {
			const std::vector<Vertex>& vertices = meshes[i]->getVertices();
			for (std::size_t j = 0; j < vertices.size(); j++) {
				result[count].position = vertices[j].position;
				result[count].color = vertices[j].color;
				count++;
			}
		}
	}

	std::size_t Renderer::getNumVertices(std::vector<Mesh*>& meshes)
	{
		std::size_t count = 0;
		for (std::size_t i = 0; i < meshes.size(); i++)
			count += meshes[i]->getVertexCount();

		return count;
	}

	void Renderer::calcIndices(std::vector<Mesh*>& meshes, std::vector<GLushort>& result)
	{
		result.resize(getNumIndices(meshes));
		std::size_t count = 0;
		GLushort offset = 0;

		for (std::size_t i = 0; i < meshes.size(); i++) {
			const std::vector<GLushort>& indices = meshes[i]->getIndices();
			std::copy(indices.begin(), indices.end(), result.begin() + count);
			for (std::size_t j = 0; j < indices.size(); j++)
				result[count + j] = indices[j] + offset;

			count += indices.size();
			offset += (GLushort)meshes[i]->getVertexCount();
		}
	}

	std::size_t Renderer::getNumIndices(std::vector<Mesh*>& meshes)
	{
		std::size_t count = 0;

		for (std::size_t i = 0; i < meshes.size(); i++)
			count += meshes[i]->getIndexCount();

		return count;
	}

	void Renderer::setupDraw(std::vector<Vertex>& vertices)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		Shader* defaultShader = Shader::getDefault();
		defaultShader->use();
		setVertexAttribPointer(defaultShader->getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(vertices[0].position));
		setVertexAttribPointer(defaultShader->getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(vertices[0].color));
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

	void Renderer::cleanupDraw()
	{
		Shader* defaultShader = Shader::getDefault();
		glDisableVertexAttribArray(defaultShader->getAttributeLocation("a_vColor"));
		glDisableVertexAttribArray(defaultShader->getAttributeLocation("a_vPosition"));
	}
}