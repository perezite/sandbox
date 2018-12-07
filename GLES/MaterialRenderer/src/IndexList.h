#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Vertex.h"
#include <vector>

namespace sb
{
	struct DrawableInfo {
		DrawableInfo(std::size_t positionInDrawableList_ = 0, std::size_t positionInIndexList_ = 0, GLushort offsetInIndexList_ = 0)
			: positionInDrawableList(positionInDrawableList_), positionInIndexList(positionInIndexList_), offsetInIndexList(offsetInIndexList_)
		{ }

		std::size_t positionInDrawableList;
		std::size_t positionInIndexList;
		GLushort offsetInIndexList;
	};

	class IndexList
	{
	public:
		IndexList()
		{
			reset();
		}

		void remove(Drawable* drawable);

		void add(Drawable* drawable);

		void calculate();

		void print();

		const std::vector<GLushort>& getIndices() const { return m_indices; }

	protected:
		void addDrawables();

		void removeDrawables();

		void resizeIndices();

		void recalcIndices();

		void reset();

	private:
		std::vector<Drawable*> m_drawables;

		std::map<Drawable*, DrawableInfo> m_drawableInfos;

		std::vector<Drawable*> m_drawablesToAdd;

		std::vector<Drawable*> m_drawablesToRemove;

		int m_numIndicesToAdd;

		std::vector<GLushort> m_indices;

		std::size_t m_firstAffectedDrawable;
	};
}