#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexList.h" 
#include "DrawBatch.h"
#include "Material.h"
#include <map>


namespace sb
{
	class Renderer
	{
	public:
		void add(Drawable* drawable);

		void remove(Drawable* drawable);

		void add(DrawBatch* batch) { m_batchesToAdd.push_back(batch); }

		void draw();

		void reset();

	protected: 
		void draw(DrawBatch* batch);

		void cleanupMainBatches();

		void addBatches();

		void setupDraw(DrawBatch* batch);

		void setupVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void drawBatch(DrawBatch* batch);

		void checkGLErrors();

		void cleanupDraw(DrawBatch* batch);

	private:
		std::map<Material, DrawBatch> m_mainBatches;

		std::vector<DrawBatch*> m_batches;

		std::vector<DrawBatch*> m_batchesToAdd;
	};
}