#include "Renderer.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	std::size_t Renderer::m_numDrawCalls = 0;
	
	void Renderer::render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& state)
	{
		if (vertices.empty())
			return;

		Shader* shader = selectShader(state);
		setup(shader, vertices, state);
		drawVertices(vertices, primitiveType);
		cleanup(shader, state);
	}

	std::size_t Renderer::getNumDrawCalls()
	{
		return m_numDrawCalls;
	}

	void Renderer::resetStatistics() 
	{
		m_numDrawCalls = 0;
	}

	Shader* Renderer::selectShader(const DrawStates& state)
	{
		if (state.shader)
			return state.shader;

		if (state.texture)
			return &Shader::getDefaultTextured();

		else
			return &Shader::getDefault();
	}

	inline void enablePointSize() {
		#ifdef WIN32
			GL_CHECK(glEnable(GL_PROGRAM_POINT_SIZE));
		#endif
	}

	void Renderer::setup(Shader* shader, const std::vector<Vertex>& vertices, const DrawStates& state)
	{
		GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GL_CHECK(glEnable(GL_BLEND));
		enablePointSize();
		GL_CHECK(glActiveTexture(GL_TEXTURE0));

		shader->use();
		setupShaderUniforms(shader, state);
		setupShaderAttributes(shader, vertices, state);
	}

	void Renderer::setupShaderUniforms(Shader* shader, const DrawStates & state)
	{
		shader->setMatrix3("transform", state.transform.getMatrix());
		if (state.texture) {
			shader->setInteger("texture", 0);
			shader->setMatrix3("texTransform", state.textureTransform.getMatrix());
			state.texture->bind();
		}
	}

	void Renderer::setupShaderAttributes(Shader * shader, const std::vector<Vertex>& vertices, const DrawStates & state)
	{
		GLvoid* position = (GLvoid*) &(vertices[0].position);
		GLvoid* color = (GLvoid*) &(vertices[0].color);
		GLvoid* texCoords = (GLvoid*) &(vertices[0].texCoords);
		setShaderAttribute(shader->getAttributeLocation("position"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), position);
		setShaderAttribute(shader->getAttributeLocation("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), color);
		if (state.texture)
			setShaderAttribute(shader->getAttributeLocation("texCoords"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), texCoords);
	}

	void Renderer::setShaderAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* value)
	{
		GL_CHECK(glVertexAttribPointer(index, size, type, normalized, stride, value));
		GL_CHECK(glEnableVertexAttribArray(index));
	}

	void Renderer::drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType)
	{	
		GL_CHECK(glDrawArrays((GLenum)primitiveType, 0, vertices.size()));

		m_numDrawCalls++;
	}

	void Renderer::cleanup(Shader* shader, const DrawStates& state)
	{
		if (state.texture)
			GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("texCoords")));
		GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("color")));
		GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("position")));

	}
}