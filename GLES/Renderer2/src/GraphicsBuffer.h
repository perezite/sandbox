#pragma once

#include "GL.h"
#ifdef  WIN32
	#define ORB_GL
#else
	#define ORB_GLES
#endif 

#include <map>

namespace sb
{
	namespace renderer2
	{
		struct VertexAttribPointer
		{
			GLint size; GLenum type; GLboolean normalized; GLsizei stride; GLvoid* pointer;
		};

		class GraphicsBuffer
		{
		public:
			virtual ~GraphicsBuffer();

			void init();

			void bindVertexBuffer();

			void bindIndexBuffer();

			void setVertexData(GLsizeiptr size, const GLvoid* data, GLenum usage);

			void setIndexData(GLsizeiptr size, const GLvoid* data, GLenum usage);

			void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

			void setupVao();

			void enable();

		protected:
			void setupVertexAttribPointers();

		private:
			#ifdef ORB_GL
				GLuint m_vao;

				bool m_isVaoSetup;
			#endif	

			GLuint m_vbo;

			GLuint m_ibo;

			std::map<GLuint, VertexAttribPointer> m_vertexAttribPointers;
		};
	}
}