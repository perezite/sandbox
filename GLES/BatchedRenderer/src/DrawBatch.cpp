#include "Drawable.h"
#include "Renderer.h"
#include "DrawBatch.h"
#include <algorithm>

namespace sb
{
	void DrawBatch::add(Drawable* drawable)
	{
		m_drawablesToAdd.push_back(drawable);
		m_numVerticesToAdd += drawable->mesh.getVertexCount();
		m_indexList.add(drawable);
	}

	void DrawBatch::remove(Drawable* drawable)
	{
		m_drawablesRemove.push_back(drawable);
		m_numVerticesToAdd -= drawable->mesh.getVertexCount();
		m_indexList.remove(drawable);
	}

	void DrawBatch::calculate()
	{
		addDrawables();
		removeDrawables();

		m_indexList.calculate();
		resizeVertices();
		calcVertices();

		reset();
	}

	void DrawBatch::addDrawables()
	{
		m_drawables.insert(m_drawables.end(), m_drawablesToAdd.begin(), m_drawablesToAdd.end());
	}

	void DrawBatch::removeDrawables()
	{
		std::vector<Drawable*>::iterator it = m_drawables.end();
		for (std::size_t i = 0; i < m_drawablesRemove.size(); i++)
			it = std::remove(m_drawables.begin(), it, m_drawablesRemove[i]);

		m_drawables.erase(it, m_drawables.end());
	}

	void DrawBatch::resizeVertices()
	{
		m_vertices.resize(m_vertices.size() + m_numVerticesToAdd);
	}

	void DrawBatch::calcVertices()
	{
		std::size_t counter = 0;
		for (std::size_t i = 0; i < m_drawables.size(); i++)
		{
			for (std::size_t j = 0; j < m_drawables[i]->mesh.getVertexCount(); j++) {
				m_vertices[counter].position = m_drawables[i]->transform * m_drawables[i]->mesh[j].position;
				m_vertices[counter].color = m_drawables[i]->mesh[j].color;
				counter++;
			}
		}
	}

	void DrawBatch::reset()
	{
		m_drawablesToAdd.clear();
		m_drawablesRemove.clear();
		m_numVerticesToAdd = 0;
	}
}