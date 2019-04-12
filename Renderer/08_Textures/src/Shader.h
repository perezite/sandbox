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
		{ }

		Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

		static Shader& getDefault();

		GLuint getAttributeLocation(std::string attribute);

		void setUniformMatrix3(std::string uniformName, const float* matrix3);

		void loadFromMemory(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

		void loadFromAsset(const std::string& vertexShaderAssetPath, const std::string& fragmentShaderCode);

		void use();

		void destroy();

	protected:
		GLuint compile(std::string shaderCode, GLenum type);

		void link();

		GLuint getUniformLocation(std::string uniform);

		static std::string getDefaultVertexShaderCode();

		static std::string getDefaultFragmentShaderCode();

	private:
		GLuint m_handle;

		std::map<std::string, GLuint> m_attributeLocations;

		std::map<std::string, GLuint> m_uniformLocations;
	};
}