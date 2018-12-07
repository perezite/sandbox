#pragma once

#include "Material.h"
#include "Drawable.h"
#include <vector>

namespace sb
{
	class DrawBatch
	{
	public:
		DrawBatch() 
			: m_numVerticesToAdd(0)
		{ }

		inline const Drawable* operator[](std::size_t index) const { return m_drawables[index]; }

		inline std::size_t getDrawableCount() const { return m_drawables.size(); }

		inline const std::vector<Vertex>& getVertices() const { return m_vertices; }

		inline const std::vector<GLushort>& getIndices() const { return m_indexList.getIndices(); }

		inline Material& getMaterial() { return m_material; }

		void add(Drawable* drawable);

		void remove(Drawable* drawable);

		void add(Drawable& drawable) { add(&drawable); }

		void remove(Drawable& drawable) { remove(&drawable); }

		void calculate();

	protected:
		void addDrawables();

		void removeDrawables();

		void resizeVertices();

		void calcVertices();

		void reset();

	private:
		Material m_material;

		std::vector<Drawable*> m_drawables;

		std::vector<Vertex> m_vertices;

		std::vector<Drawable*> m_drawablesToAdd;

		std::vector<Drawable*> m_drawablesRemove;

		std::size_t m_numVerticesToAdd;

		IndexList m_indexList;
	};
}