#pragma once

#include "GL.h"
#include <map>
#include <string>


namespace sb
{
	class Shader
	{
	public:
		Shader()
		{ }

		void init();

		GLuint getAttributeLocation(std::string attribute);

		void use();

		void destroy();

	protected:
		std::string getVertexShaderSource();

		std::string getFragmentShaderSource();

		GLuint compile(std::string shaderCode, GLenum type);

		void link();

	private:
		GLuint m_shader;

		std::map<std::string, GLuint> m_attributeLocations;
	};
}