#pragma once

#ifdef  WIN32
#include <gl/glew.h>
#include <SDL2/SDL_opengl.h>
#include <gl/glu.h>
#else
#include <GLES2/gl2.h>	
#include <GLES2/gl2ext.h>
#endif 
#include <map>
#include <string>


namespace sb
{
	namespace renderer2
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
}