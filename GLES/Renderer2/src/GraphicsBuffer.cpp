#include "GraphicsBuffer.h"

namespace sb
{
	namespace renderer2
	{
		void GraphicsBuffer::init()
		{
			#ifdef ORB_GL
				glGenVertexArrays(1, &m_vao);
			#endif
			glGenBuffers(1, &m_vbo);
			glGenBuffers(1, &m_ibo);
		}

		GraphicsBuffer::~GraphicsBuffer()
		{
			glDeleteBuffers(1, &m_ibo);
			glDeleteBuffers(1, &m_vbo);

			#ifdef ORB_GL
				glDeleteVertexArrays(1, &m_vao);
			#endif
		}
	
		void GraphicsBuffer::bindVertexBuffer()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		}

		void GraphicsBuffer::bindIndexBuffer()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		}

		void GraphicsBuffer::setVertexData(GLsizeiptr size, const GLvoid* data, GLenum usage)
		{
			glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		}

		void GraphicsBuffer::setIndexData(GLsizeiptr size, const GLvoid* data, GLenum usage)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		}

		void GraphicsBuffer::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
		{
			m_vertexAttribPointers[index] = VertexAttribPointer{ size, type, normalized, stride, pointer };
		}

		void GraphicsBuffer::enable()
		{
			#ifdef ORB_GL
				glBindVertexArray(m_vao);
				if (m_isVaoSetup == false) {
					setupVao();
					m_isVaoSetup = true;
				}
			#else
				setupVertexAttribPointers();
			#endif		
		}

		void GraphicsBuffer::setupVao()
		{
			bindVertexBuffer();
			bindIndexBuffer();
			setupVertexAttribPointers();
		}

		void GraphicsBuffer::setupVertexAttribPointers()
		{
			for(std::map<GLuint, VertexAttribPointer>::iterator it = m_vertexAttribPointers.begin(); it != m_vertexAttribPointers.end(); ++it)
			{
				GLuint index = it->first;
				VertexAttribPointer vap = it->second;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, vap.size, vap.type, vap.normalized, vap.stride, vap.pointer);
			}
			glEnableVertexAttribArray(0);
		}
	}
}

