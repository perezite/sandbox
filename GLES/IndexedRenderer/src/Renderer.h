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
		std::size_t count;
		std::size_t offset;
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
		void addDrawables(std::size_t& startDrawable, std::size_t& numAddedIndices);

		void removeDrawables(std::size_t& startDrawable, std::size_t& numRemovedIndices);

		void resizeIndices(std::size_t numAddedIndices, std::size_t numRemovedIndices);

		void recalcIndices(std::size_t start);

		void print();

	private:
		std::vector<Drawable*> m_drawables;

		std::vector<IndexInfo> m_indexInfos;

		std::map<Drawable*, std::size_t> m_drawablePositions;

		std::vector<Drawable*> m_drawablesToAdd;

		std::vector<Drawable*> m_drawablesToRemove;

		std::vector<GLuint> m_indices;
	};
}