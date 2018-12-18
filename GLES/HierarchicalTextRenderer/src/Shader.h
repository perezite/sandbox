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

		void loadFromMemory(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		
		GLuint getAttributeLocation(const std::string& attribute);

		inline GLuint getShaderId() const { return m_shader; }

		void use();

		void destroy();

	public: 
		static Shader* getDefault();

	protected:
		GLuint compile(const std::string& shaderCode, GLenum type);

		void link();

		static std::string getDefaultVertexShaderSource();

		static std::string getDefaultFragmentShaderSource();

	private:
		GLuint m_shader;

		std::map<std::string, GLuint> m_attributeLocations;
	};
}