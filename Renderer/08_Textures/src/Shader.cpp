#include "Shader.h"
#include "Logger.h"

#define SB_SHADER_CODE(x) #x

namespace sb
{
	Shader::Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) 
	{
		loadFromMemory(vertexShaderCode, fragmentShaderCode);
	}

	void Shader::loadFromMemory(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) 
	{
		m_handle = glCreateProgram();
		SB_ERROR_IF(m_handle == 0) << "Allocating shader failed" << std::endl;

		GLuint vertexShader = compile(vertexShaderCode, GL_VERTEX_SHADER);
		GLuint fragmentShader = compile(fragmentShaderCode, GL_FRAGMENT_SHADER);

		glAttachShader(m_handle, vertexShader);
		glAttachShader(m_handle, fragmentShader);

		link();

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader& Shader::getDefault() 
	{
		static Shader defaultShader;
		defaultShader.loadFromMemory(getDefaultVertexShaderCode(), getDefaultFragmentShaderCode());
		return defaultShader;
	}

	GLuint Shader::getAttributeLocation(std::string attribute) 
	{
		if (m_attributeLocations.find(attribute) == m_attributeLocations.end()) {
			GLuint location = glGetAttribLocation(m_handle, attribute.c_str());
			m_attributeLocations[attribute] = location;
			return location;
		}

		return m_attributeLocations[attribute];
	}

	void Shader::setUniformMatrix3(std::string uniformName, const float* matrix3)
	{
		GLuint uniformLocation = getUniformLocation(uniformName);

		glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, matrix3);
	}

	void Shader::use()
	{
		glUseProgram(m_handle);
	}

	void Shader::destroy()
	{
		glDeleteProgram(m_handle);
	}

	GLuint Shader::compile(std::string shaderCode, GLenum type)
	{
		GLint compiled;
		GLuint shader = glCreateShader(type);

		if (shader != 0) {
			const char* shaderCodeStr = shaderCode.c_str();
			glShaderSource(shader, 1, &shaderCodeStr, NULL);
			glCompileShader(shader);

			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1) {
					char* infoLog = new char[infoLen];
					glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
					SB_ERROR() << "error compiling shader: " << infoLog << std::endl;
					delete[] infoLog;
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}

		return shader;
	}

	void Shader::link()
	{
		glLinkProgram(m_handle);
		GLint linked;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &linked);
		if (!linked) {
			GLint infoLen = 0;
			glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1) {
				char* infoLog = new char[infoLen];
				glGetProgramInfoLog(m_handle, infoLen, NULL, infoLog);
				SB_ERROR() << "Error linking shader program: " << std::endl << infoLog << std::endl;
				delete[] infoLog;
			}

			glDeleteProgram(m_handle);
		}
	}

	GLuint Shader::getUniformLocation(std::string uniform)
	{
		if (m_uniformLocations.find(uniform) == m_uniformLocations.end()) {
			GLuint location = glGetUniformLocation(m_handle, uniform.c_str());
			m_uniformLocations[uniform] = location;
			return location;
		}

		return m_uniformLocations[uniform];
	}

	std::string Shader::getDefaultVertexShaderCode()
	{
		return SB_SHADER_CODE (
			attribute vec2 position;																\n
			attribute vec4 color;																	\n
			uniform mat3 transform;																	\n
			varying vec4 v_color;																	\n
			vec3 transformedPosition;																\n
			void main()																				\n
			{																						\n							
				transformedPosition = transform * vec3(position.x, position.y, 1);					\n
				gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0, 1 );			\n
				v_color = color;																	\n
			}																						\n
		);
	}

	std::string Shader::getDefaultFragmentShaderCode()
	{
		return SB_SHADER_CODE(
			#version 100									\n
			precision mediump float;						\n
			varying vec4 v_color;		 					\n
			void main()										\n
			{												\n
				gl_FragColor = v_color;						\n
			}												\n
		);
	}
}