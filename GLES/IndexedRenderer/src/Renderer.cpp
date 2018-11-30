#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>

namespace sb
{
	void Renderer::init()
	{
		
	}

	void Renderer::remove(Drawable* drawable)
	{
		if (m_drawableInfos.find(drawable) == m_drawableInfos.end() && 
			std::find(m_drawablesToAdd.begin(), m_drawablesToAdd.end(), (drawable)) == m_drawables.end())
			Error().die() << "Trying to remove a non-existing drawable, aborting";

		m_drawablesToRemove.push_back(drawable);
		m_numIndicesToAdd -= drawable->mesh.getIndexCount();
		m_firstAffectedDrawable = std::min(m_firstAffectedDrawable, m_drawableInfos[drawable].positionInDrawableList);
	}

	void Renderer::add(Drawable* drawable)
	{
		if (m_drawableInfos.find(drawable) != m_drawableInfos.end() && 
			std::find(m_drawablesToRemove.begin(), m_drawablesToRemove.end(), drawable) == m_drawables.end())
			Error().die() << "Trying to add an already existing drawable, aborting";

		m_drawablesToAdd.push_back(drawable);
		m_numIndicesToAdd += drawable->mesh.getIndexCount();
	}

	void Renderer::render()
	{
		addDrawables();
		removeDrawables();
		resizeIndices();
		recalcIndices();

		print();
		cleanupRender();
 	}

	void Renderer::addDrawables()
	{
		if (m_drawablesToAdd.empty()) 
			return;

		for (std::size_t i = 0; i < m_drawablesToAdd.size(); i++) 
			m_drawables.push_back(m_drawablesToAdd[i]);
	}

	void Renderer::removeDrawables() 
	{
		std::vector<Drawable*>::iterator it = m_drawables.end();
		for (std::size_t i = 0; i < m_drawablesToRemove.size(); i++) {
			it = std::remove(m_drawables.begin(), it, m_drawablesToRemove[i]);
			m_drawableInfos.erase(m_drawablesToRemove[i]);
		}

		m_drawables.erase(it, m_drawables.end());
	}


	void Renderer::resizeIndices()
	{
		m_indices.resize(m_indices.size() + m_numIndicesToAdd);
	}

	void Renderer::recalcIndices()
	{
		bool allAffected = m_firstAffectedDrawable == 0;
		Drawable* lastUnaffectedDrawable = allAffected ? NULL : m_drawables[m_firstAffectedDrawable - 1];
		DrawableInfo* lastUnaffectedInfo = allAffected ? NULL : &m_drawableInfos[lastUnaffectedDrawable];
		std::size_t positionInDrawableList = allAffected ? 0 : m_firstAffectedDrawable;
		std::size_t positionInIndexList = allAffected ? 0 : lastUnaffectedInfo->positionInIndexList + lastUnaffectedDrawable->mesh.getIndexCount();
		GLushort offsetInIndexList = allAffected ? 0 : lastUnaffectedInfo->offsetInIndexList + (GLushort)lastUnaffectedDrawable->mesh.getVertexCount();

		for (std::size_t i = m_firstAffectedDrawable; i < m_drawables.size(); i++) {
			const std::vector<GLushort>& indices = m_drawables[i]->mesh.getIndices();
			for (std::size_t j = 0; j < indices.size(); j++)
				m_indices[positionInIndexList + j] = offsetInIndexList + indices[j];

			Drawable* drawable = m_drawables[i];
			m_drawableInfos[drawable] = DrawableInfo(positionInDrawableList, positionInIndexList, offsetInIndexList);
			positionInDrawableList += 1;
			positionInIndexList += drawable->mesh.getIndexCount();
			offsetInIndexList += (GLushort)drawable->mesh.getVertexCount();
		}
	}

	void Renderer::print()
	{
		std::cout << "number of drawables: " << m_drawables.size() << std::endl;
		std::cout << "indices:" << std::endl;
		for (std::size_t i = 0; i < m_drawables.size(); i++) {
			std::size_t startPos = m_drawableInfos[m_drawables[i]].positionInIndexList;
			std::size_t numIndices = m_drawables[i]->mesh.getIndexCount();
			for (std::size_t j = 0; j < numIndices; j++)
				std::cout << m_indices[startPos + j] << " ";

			std::cout << "| ";
		}

		std::cout << std::endl;
	}

	void Renderer::cleanupRender()
	{
		m_drawablesToAdd.clear();
		m_numIndicesToAdd = 0;
		m_drawablesToRemove.clear();
		m_firstAffectedDrawable = m_drawables.size();
	}
}