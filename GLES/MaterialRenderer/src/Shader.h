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

		inline GLuint getShaderId() const { return m_shader; }

		GLuint getAttributeLocation(std::string attribute);

		void use();

		void destroy();

	public:
		static Shader& getDefault();

	protected:
		std::string getVertexShaderSource();

		std::string getFragmentShaderSource();

		GLuint compile(std::string shaderCode, GLenum type);

		void link();

	private:
		GLuint m_shader;

		std::map<std::string, GLuint> m_attributeLocations;
	};

	bool operator <(Shader& left, Shader& right);

	bool operator ==(Shader& left, Shader& right);
}