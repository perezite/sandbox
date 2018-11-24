#pragma once

#ifdef  WIN32
	#include <gl/glew.h>
	#include <SDL2/SDL_opengl.h>
	#include <gl/glu.h>
#else
	#include <GLES2/gl2.h>	
	#include <GLES2/gl2ext.h>
#endif 
#include <SDL2/SDL.h>
#include <string>

namespace sb 
{
	namespace renderer1
	{
		class RenderUtil
		{
		public:
			static void initSDL();

			static bool updateInput();

			static void flip();

			static void close();

			static GLuint createShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

			static void activateShader(GLuint shader);

			static void attachVertexArrayToShaderAttribute(GLuint shaderAttributeLocation, GLint elementCount, GLenum elementType, GLsizei strideCount, const GLvoid* firstElement);

			static void detachVertexArrayFromShaderAttribute(GLuint shaderAttributeLocation);

		public:
			static std::string getDiffuseVertexShaderCode();

			static std::string getDiffuseFragmentShaderCode();

		protected:
			static GLuint compileShader(std::string shaderCode, GLenum type);

			static void linkShader(GLuint shader);

		private:
			static SDL_Window* m_sdlWindow;

			static SDL_GLContext m_glContext;

			static GLuint m_shader;
		};

	}
}