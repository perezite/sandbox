#include "Shader.h"

#include <iostream>

namespace sb
{
	namespace renderer2
	{
		void Shader::init()
		{
			m_shader = glCreateProgram();
			if (m_shader == 0) {
				std::cout << "error creating shader program" << std::endl;
				std::cin.get();
			}

			std::string vertexShaderCode = getVertexShaderSource();
			std::string fragmentShaderCode = getFragmentShaderSource();
			GLuint vertexShader = compile(vertexShaderCode, GL_VERTEX_SHADER);
			GLuint fragmentShader = compile(fragmentShaderCode, GL_FRAGMENT_SHADER);

			glAttachShader(m_shader, vertexShader);
			glAttachShader(m_shader, fragmentShader);
			link();
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		GLuint Shader::getAttributeLocation(std::string attribute) 
		{
			GLuint location = glGetAttribLocation(m_shader, attribute.c_str());
			m_attributeLocations[attribute] = location;
			return location;
		}

		void Shader::use()
		{
			glUseProgram(m_shader);
		}

		void Shader::destroy()
		{
			glDeleteProgram(m_shader);
		}

		std::string Shader::getVertexShaderSource()
		{
			return
				"attribute vec2 a_vPosition;										\n"
				"attribute vec4 a_vColor;											\n"
				"varying vec4 v_vColor;												\n"
				"void main()														\n"
				"{																	\n"
				"   gl_Position = vec4(a_vPosition.x, a_vPosition.y, 0 , 1 );		\n"
				"	v_vColor = a_vColor;											\n"
				"}";
		}

		std::string Shader::getFragmentShaderSource()
		{
			return
				"#version 100										\n"
				"precision mediump float;						\n"
				"varying vec4 v_vColor;		 					\n"
				"void main()									\n"
				"{												\n"
				"  gl_FragColor = v_vColor;						\n"
				"}												\n";
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
						std::cout << "error compiling shader: " << infoLog << std::endl;
						std::cin.get();
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
			glLinkProgram(m_shader);
			GLint linked;
			glGetProgramiv(m_shader, GL_LINK_STATUS, &linked);
			if (!linked) {
				GLint infoLen = 0;
				glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1) {
					char* infoLog = new char[infoLen];
					glGetProgramInfoLog(m_shader, infoLen, NULL, infoLog);
					std::cout << "Error linking shader program: " << std::endl << infoLog << std::endl;
					std::cin.get();
					delete[] infoLog;
				}

				glDeleteProgram(m_shader);
			}
		}
	}
}