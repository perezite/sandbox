#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include <vector>
#include <list>

namespace sb
{
	struct IndexInfo {
		std::size_t position;
		GLuint offset;
	};

	class Renderer
	{
	public:
		Renderer()
		{ }

		void init();

		void add(Drawable* drawable);

		void remove(Drawable* drawable);

		void render();

	protected:
		void calculateIndices();

		void print();

		Drawable* removeDrawables();

		Drawable* addDrawables();

		void resizeIndices();

		void shrinkIndices();

		std::size_t countRemovedIndices();

		std::size_t countAddedIndices();

		void recalcIndices(Drawable* start);

	private:
		std::map<Drawable*, IndexInfo> m_drawables;

		std::vector<Drawable*> m_drawablesToAdd;

		std::vector<Drawable*> m_drawablesToRemove;

		std::vector<GLuint> m_indices;
	};
}