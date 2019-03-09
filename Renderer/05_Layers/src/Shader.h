#pragma once

#include "GL.h"
#include <map>
#include <string>


namespace sb
{
	class Shader
	{
	public:
		Shader();

		GLuint getAttributeLocation(std::string attribute);

		void setUniformMatrix3(std::string uniformName, const float* matrix3);

		void setUniformFloat(std::string uniformName, float value);

		void use();

		void destroy();

	protected:
		std::string getVertexShaderSource();

		std::string getFragmentShaderSource();

		GLuint getUniformLocation(std::string uniform);

		GLuint compile(std::string shaderCode, GLenum type);

		void link();

	private:
		GLuint m_shader;

		std::map<std::string, GLuint> m_attributeLocations;

		std::map<std::string, GLuint> m_uniformLocations;
	};
}