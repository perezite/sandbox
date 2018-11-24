#pragma once

#include "Window.h"
#include "Drawable.h"
#include "GraphicsBuffer.h"
#include "Shader.h"
#include "Stopwatch.h"

#include <string>
#include <map>
#include <vector>

namespace sb 
{
	namespace renderer2 
	{
		// This sandbox implements the triangle with vertex buffers and vertex array objects. 
		// References:	https://learnopengl.com/Getting-started/Hello-Triangle
		//				https://www.youtube.com/watch?v=ImtWD_9OAeY&t=2s
		//				http://www.learnopengles.com/android-lesson-seven-an-introduction-to-vertex-buffer-objects-vbos/
		class Renderer2
		{
		public:
			static const unsigned int NumTrianglesHorz;
			static const unsigned int NumTrianglesVert;

		public:
			static void run();

		protected:
			static void initGL();

			static void initActors();

			static void logPerformance();

			static void render();

			static void calcVertices();

			static std::size_t getNumVertices();

			static void calcIndices();

			static std::size_t getNumIndices();

			static void setupBuffer();

			static void display();

			static void checkGLErrors();

			static void prepareDisplay();

			static void close();

		private:
			static Window m_window;

			static Shader m_shader;

			static std::vector<Drawable*> m_triangles;

			static GraphicsBuffer m_buffer;

			static std::vector<Vertex> m_vertices;

			static std::vector<GLushort> m_indices;

			static bool m_indicesNeedUpdate;
		};
	}
}