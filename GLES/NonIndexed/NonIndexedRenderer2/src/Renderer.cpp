#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	void Renderer::render(Drawable& drawable, Shader* shader)
	{
		Shader* theShader = shader != NULL ? shader : &getDefaultShader();
		Material material(theShader);
		m_batches[material].push_back(&drawable);
	}

	void Renderer::display()
	{
		for (BatchIter it = m_batches.begin(); it != m_batches.end(); it++)
			display(it->second, it->first);

		std::cout << "draw calls " << m_batches.size() << std::endl;

		m_batches.clear();
	}

	Shader& Renderer::getDefaultShader()
	{
		static Shader shader;
		return shader;
	}

	void Renderer::display(std::vector<Drawable*>& drawables, const Material& material)
	{
		std::vector<Vertex> vertices;
		calcVertices(drawables, vertices);

		setupDraw(vertices, material);
		draw(vertices);
		cleanupDraw(material);
	}

	void Renderer::calcVertices(std::vector<Drawable*>& drawables, std::vector<Vertex>& result)
	{
		result.resize(getNumVertices(drawables));
		
		unsigned int counter = 0;
		for (std::size_t i = 0; i < drawables.size(); i++) {
			for (std::size_t j = 0; j < drawables[i]->mesh.getVertexCount(); j++) {
				result[counter].position = drawables[i]->transform * drawables[i]->mesh[j].position;
				result[counter].color = drawables[i]->mesh[j].color;
				counter++;
			}
		}
	}

	std::size_t Renderer::getNumVertices(std::vector<Drawable*>& drawables)
	{
		std::size_t numVertices = 0;
		for (std::size_t i = 0; i < drawables.size(); i++)
			numVertices += drawables[i]->mesh.getVertexCount();

		return numVertices;
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

	void Renderer::draw(std::vector<Vertex>& vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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