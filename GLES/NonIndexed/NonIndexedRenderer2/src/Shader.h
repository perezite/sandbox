#pragma once

#include "GL.h"
#include <map>
#include <string>


namespace sb
{
	class Shader
	{
	public:
		void loadFromMemory(std::string vertexShaderSource, std::string fragmentShaderSource);
		
		void loadDefault();

		GLuint getAttributeLocation(std::string attribute);

		inline GLuint getShaderId() const { return m_shader; }

		void use();

		void destroy();

	protected:
		GLuint compile(std::string shaderCode, GLenum type);

		void link();

		std::string getDefaultVertexShaderSource();

		std::string getDefaultFragmentShaderSource();

	private:
		GLuint m_shader;

		std::map<std::string, GLuint> m_attributeLocations;
	};

	bool operator <(const Shader& left, const Shader& right);
}