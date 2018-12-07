#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexList.h" 
#include "DrawBatch.h"

namespace sb
{
	class Renderer
	{
	public:
		void add(Drawable* drawable) { m_mainBatch.add(drawable); }

		void remove(Drawable* drawable) { m_mainBatch.remove(drawable); }

		void add(DrawBatch* batch) { m_batchesToAdd.push_back(batch); }

		void render();

		void reset();

	protected: 
		void render(DrawBatch* batch);

		void addBatches();

		void setupDraw(DrawBatch* batch);

		void setupVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void draw(DrawBatch* batch);

		void checkGLErrors();

		void cleanupDraw(DrawBatch* batch);

	private:
		DrawBatch m_mainBatch;

		std::vector<DrawBatch*> m_batches;

		std::vector<DrawBatch*> m_batchesToAdd;
	};
}