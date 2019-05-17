#include "Renderer.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	std::size_t Renderer::m_numDrawCalls = 0;
	
	void Renderer::render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states)
	{
		if (vertices.empty())
			return;

		setup(vertices, states);
		drawVertices(vertices, primitiveType);
		cleanup(states);
	}

	void Renderer::resetStatistics() 
	{
		m_numDrawCalls = 0;
	}

	std::size_t Renderer::getNumDrawCalls()
	{
		return m_numDrawCalls;
	}

	void Renderer::setup(const std::vector<Vertex>& vertices, const DrawStates& states)
	{
		GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glActiveTexture(GL_TEXTURE0));

		Shader* shader = selectShader(states);
		shader->use();

		if (states.texture) 
			states.texture->bind();

		GLvoid* position = (GLvoid*) &(vertices[0].position);
		GLvoid* color = (GLvoid*) &(vertices[0].color); 
		GLvoid* texCoords = (GLvoid*) &(vertices[0].texCoords);
		setVertexAttribPointer(shader->getAttributeLocation("position"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), position);
		setVertexAttribPointer(shader->getAttributeLocation("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), color);
		if (states.texture)
			setVertexAttribPointer(shader->getAttributeLocation("texCoords"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), texCoords);

		shader->setMatrix3("transform", states.transform.getMatrix());
		if (states.texture) {
			shader->setInteger("texture", 0);
			shader->setMatrix3("texTransform", states.texture->getTexTransform());
		}
	}

	Shader* Renderer::selectShader(const DrawStates& states) 
	{
		if (states.shader)
			return states.shader;

		if (states.texture)
			return &Shader::getDefaultTextured();

		else
			return &Shader::getDefault();
	}

	void Renderer::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
	{
		GL_CHECK(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
		GL_CHECK(glEnableVertexAttribArray(index));
	}

	void Renderer::drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType)
	{	
		GL_CHECK(glDrawArrays((GLenum)primitiveType, 0, vertices.size()));

		m_numDrawCalls++;
	}

	void Renderer::cleanup(const DrawStates& states)
	{
		Shader* shader = selectShader(states);

		if (states.texture)
			GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("texCoords")));
		GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("color")));
		GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("position")));

	}
}