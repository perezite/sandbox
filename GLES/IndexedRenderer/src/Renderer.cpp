#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>

namespace sb
{
	typedef std::map<Drawable*, IndexInfo>::iterator DrawableIter;

	void Renderer::init()
	{
		
	}

	void Renderer::add(Drawable* drawable)
	{
		if(m_drawables.find(drawable) != m_drawables.end())
			return;

		m_drawablesToAdd.push_back(drawable);
	}

	void Renderer::remove(Drawable* drawable)
	{
		m_drawablesToRemove.push_back(drawable);
	}

	void Renderer::render()
	{
		calculateIndices();
		print();
	}

	void Renderer::calculateIndices()
	{
		Drawable* start = removeDrawables();
		recalcIndices(start);
		shrinkIndices();
		Drawable* start2 = addDrawables();
		recalcIndices(start);
		m_drawablesToRemove.clear();
		m_drawablesToAdd.clear();
	}

	void Renderer::shrinkIndices()
	{
		std::size_t numRemovedIndices = countRemovedIndices();
		m_indices.resize(m_indices.size() + numRemovedIndices);
	}

	void Renderer::print()
	{
		std::cout << "indices:" << std::endl;
		for (std::size_t i = 0; i < m_indices.size(); i++)
			std::cout << m_indices[i] << " ";
		std::cout << std::endl;

		std::cout << "press any key to continue" << std::endl;
		std::cin.get();
	}

	Drawable* Renderer::removeDrawables() 
	{
		DrawableIter start = m_drawables.end();

		for (std::size_t i = 0; i < m_drawablesToRemove.size(); i++) {
			DrawableIter it = m_drawables.find(m_drawablesToRemove[i]);
			std::size_t position = it->second.position;
			start = position < start->second.position ? it : start;
			m_drawables.erase(it);
		}

		return start->first;
	}

	Drawable* Renderer::addDrawables() {
		if (m_drawablesToAdd.empty())
			return NULL;

		IndexInfo indexInfo = m_drawables.empty() ? IndexInfo() : std::prev(m_drawables.end(), 1)->second;
		m_drawables[m_drawablesToAdd[0]] = IndexInfo();

		for (std::size_t i = 1; i < m_drawablesToAdd.size(); i++) {
			indexInfo.position += m_drawablesToAdd[i - 1]->mesh.getIndexCount();
			indexInfo.offset += m_drawablesToAdd[i - 1]->mesh.getLargestIndex();
			m_drawables[m_drawablesToAdd[i]] = indexInfo;
		}

		return NULL;
	}

	void Renderer::resizeIndices()
	{
		std::size_t numRemovedIndices = countRemovedIndices();
		std::size_t numAddedIndices = countAddedIndices();
		m_indices.resize(m_indices.size() + numAddedIndices - numRemovedIndices);
	}

	std::size_t Renderer::countRemovedIndices()
	{
		std::size_t numRemovedIndices = 0;
		for (std::size_t i = 0; i < m_drawablesToRemove.size(); i++)
			numRemovedIndices += m_drawablesToRemove[i]->mesh.getIndexCount();
	
		return numRemovedIndices;
	}

	std::size_t Renderer::countAddedIndices()
	{
		std::size_t numRemovedIndices = 0;
		for (std::size_t i = 0; i < m_drawablesToAdd.size(); i++)
			numRemovedIndices += m_drawablesToAdd[i]->mesh.getIndexCount();

		return numRemovedIndices;
	}

	void Renderer::recalcIndices(Drawable* start) 
	{
		DrawableIter it = std::prev(m_drawables.find(start), 1);

		for (it; it != m_drawables.end(); it++) {
			const std::vector<GLushort>& indices = it->first->mesh.getIndices();
			for (std::size_t i = 0; i < indices.size(); i++) {
				m_indices[it->second.position + i] = it->second.offset + indices[i];
			}
		}
	}
}