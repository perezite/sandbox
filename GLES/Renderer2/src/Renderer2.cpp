#include "Renderer2.h"
#include <iostream>
#include <stddef.h>

namespace sb 
{
	namespace renderer2 
	{
		const unsigned int Renderer2::NumTrianglesHorz = 100;
		const unsigned int Renderer2::NumTrianglesVert = 100;
		
		Window Renderer2::m_window;
		Shader Renderer2::m_shader;
		std::vector<Drawable*> Renderer2::m_triangles;
		GraphicsBuffer Renderer2::m_buffer;
		std::vector<Vertex> Renderer2::m_vertices;
		std::vector<GLushort> Renderer2::m_indices;
		bool Renderer2::m_indicesNeedUpdate = true;

		void Renderer2::run()
		{
			SDL_Log("Renderer2: %s %s", __DATE__, __TIME__);

			m_window.init(1500, 800);
			SDL_GL_SetSwapInterval(0);
			initGL();
			initActors();

			while (!m_window.hasQuitEvent()) {
				logPerformance();
				m_window.update();
				render();
				display();
				m_window.flip();
			}

			close();
		}

		void Renderer2::initGL()
		{
			m_shader.init();
			m_buffer.init();
		}

		void Renderer2::initActors()
		{
			float stepWidth = 2 / float(NumTrianglesHorz);
			float stepHeight = 2 / float(NumTrianglesVert);
			Vector2f size(stepWidth, stepHeight);

			for (unsigned int i = 0; i < NumTrianglesHorz; i++) {
				for (unsigned int j = 0; j < NumTrianglesVert; j++) {
					Vector2f position = Vector2f(-1 + i * stepWidth + 0.5f * stepWidth, -1 + j * stepHeight + 0.5f * stepWidth);
					m_triangles.push_back(new Drawable(position, size));
				}
			}
		}

		void Renderer2::logPerformance()
		{
			static Stopwatch stopwatch;
			static unsigned int frames = 0;

			float elapsed = stopwatch.getElapsedSeconds();
			frames++;
			if (elapsed > 1.0f) {
				float fps = frames / elapsed;
				SDL_Log("FPS: %f", fps);
				frames = 0;
				stopwatch.reset();
			}
		}

		void Renderer2::render()
		{
			calcVertices();
			if (m_indicesNeedUpdate)
				calcIndices();
			setupBuffer();

			m_indicesNeedUpdate = false;
		}

		void Renderer2::calcVertices()
		{
			m_vertices.resize(getNumVertices());

			unsigned int counter = 0;
			for (std::size_t i = 0; i < m_triangles.size(); i++) {
				for (std::size_t j = 0; j < m_triangles[i]->getMesh().getVertexCount(); j++) {
					m_vertices[counter].position = m_triangles[i]->getTransform() * m_triangles[i]->getMesh()[j].position;
					m_vertices[counter].color = m_triangles[i]->getMesh()[j].color;
					counter++;
				}
			}
		}

		std::size_t Renderer2::getNumVertices()
		{
			std::size_t numVertices = 0;
			for (std::size_t i = 0; i < m_triangles.size(); i++)
				numVertices += m_triangles[i]->getMesh().getVertexCount();

			return numVertices;
		}

		void Renderer2::calcIndices()
		{
			m_indices.resize(getNumIndices());

			unsigned int counter = 0;
			unsigned int offset = 0;
			for (std::size_t i = 0; i < m_triangles.size(); i++) {
				const std::vector<GLuint>& indices = m_triangles[i]->getMesh().getIndices();
				for (std::size_t j = 0; j < m_triangles[i]->getMesh().getIndexCount(); j++) {
					m_indices[counter] = indices[j] + offset;
					counter++;
				}
				offset += m_triangles[i]->getMesh().getVertexCount();
			}
		}

		std::size_t Renderer2::getNumIndices()
		{
			std::size_t numIndices = 0;
			for (std::size_t i = 0; i < m_triangles.size(); i++)
				numIndices += m_triangles[i]->getMesh().getIndexCount();

			return numIndices;
		}

		void Renderer2::setupBuffer()
		{
			m_buffer.bindVertexBuffer();
			m_buffer.setVertexData(m_vertices.size() * sizeof(Vertex), &(m_vertices[0]), GL_DYNAMIC_DRAW);		
			m_buffer.bindIndexBuffer();
			if (m_indicesNeedUpdate) {
				m_buffer.setIndexData(m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);
			}
		}

		void Renderer2::display()
		{
			prepareDisplay();
			m_buffer.enable();
			glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
			checkGLErrors();
		}

		void Renderer2::checkGLErrors()
		{
			GLuint error = glGetError();
			if (error != 0) {
				std::cout << error << std::endl;
				std::cin.get();
			}
		}

		void Renderer2::prepareDisplay()
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_shader.use();

			m_buffer.setVertexAttribPointer(m_shader.getAttributeLocation("a_vPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			m_buffer.setVertexAttribPointer(m_shader.getAttributeLocation("a_vColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
		}

		void Renderer2::close()
		{
			m_shader.destroy();
			m_window.destroy();
			for (std::size_t i = 0; i < m_triangles.size(); i++)
				delete m_triangles[i];
		}
	}
}

