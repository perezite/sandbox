#pragma once

#include "GL.h"
#include <map>
#include <string>
#include <iostream>

namespace sb
{
	class Shader
	{
	public:
		Shader()
		{ 
			std::cout << "Shader()" << std::endl;
		}

		Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

		void loadFromMemory(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

		Shader& getDefault();

		GLuint getAttributeLocation(std::string attribute);

		void setUniformMatrix3(std::string uniformName, const float* matrix3);

		void use();

		void destroy();

	protected:
		GLuint compile(std::string shaderCode, GLenum type);

		void link();

		GLuint getUniformLocation(std::string uniform);

		std::string getDefaultVertexShaderCode();

		std::string getDefaultFragmentShaderCode();

	private:
		std::string m_vertexShaderCode;

		std::string m_fragmentShaderCode;

		GLuint m_handle;

		std::map<std::string, GLuint> m_attributeLocations;

		std::map<std::string, GLuint> m_uniformLocations;
	};
}