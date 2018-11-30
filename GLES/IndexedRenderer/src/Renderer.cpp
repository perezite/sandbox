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

	void Renderer::add(Drawable* drawable)
	{
		if(std::find(m_drawables.begin(), m_drawables.end(), drawable) != m_drawables.end())
			return;

		m_drawablesToAdd.push_back(drawable);
	}

	void Renderer::remove(Drawable* drawable)
	{
		m_drawablesToRemove.push_back(drawable);
	}

	void Renderer::render()
	{
		std::size_t addedStart; std::size_t numAddedIndices;
		addDrawables(addedStart, numAddedIndices);

		std::size_t removedStart; std::size_t numRemovedIndices;
		removeDrawables(removedStart, numRemovedIndices);

		resizeIndices(numAddedIndices, numRemovedIndices);
		recalcIndices(std::min(addedStart, removedStart));

		m_drawablesToAdd.clear();
		m_drawablesToRemove.clear();

		print();
	}
	void Renderer::addDrawables(std::size_t& startDrawable, std::size_t& numAddedIndices)
	{
		std::size_t indexPosition = m_drawables.empty() ? 0 : m_indexInfos.rbegin()->position + (*m_drawables.rbegin())->mesh.getIndexCount();
		std::size_t indexOffset = m_drawables.empty() ? 0 : m_indexInfos.rbegin()->offset + (*m_drawables.rbegin())->mesh.getVertexCount();
		startDrawable = m_drawables.size();
		numAddedIndices = 0;
		std::size_t position = m_drawables.size();

		for (std::size_t i = 0; i < m_drawablesToAdd.size(); i++) {
			m_drawables.push_back(m_drawablesToAdd[i]);
			m_indexInfos.push_back( IndexInfo { indexPosition, m_drawablesToAdd[i]->mesh.getIndexCount(), indexOffset } );
			m_drawablePositions[m_drawablesToAdd[i]] = position;
			indexPosition += m_drawablesToAdd[i]->mesh.getIndexCount();
			indexOffset += m_drawablesToAdd[i]->mesh.getVertexCount();
			numAddedIndices += m_drawablesToAdd[i]->mesh.getIndexCount();
			position += 1;
		}
	}

	void Renderer::removeDrawables(std::size_t& startDrawable, std::size_t& numRemovedIndices) 
	{
		startDrawable = m_drawables.size();
		numRemovedIndices = 0;

		std::vector<Drawable*>::iterator it = m_drawables.end();
		for (std::size_t i = 0; i < m_drawablesToRemove.size(); i++) {
			std::size_t drawablePos = m_drawablePositions[m_drawablesToRemove[i]];
			startDrawable = std::min(startDrawable, drawablePos);
			it = std::remove(m_drawables.begin(), it, m_drawablesToRemove[i]);
			numRemovedIndices += m_indexInfos[drawablePos].count; // m_drawablesToRemove[i]->mesh.getIndexCount();
		}

		m_drawables.erase(it, m_drawables.end());
	}

	void Renderer::resizeIndices(std::size_t numAddedIndices, std::size_t numRemovedIndices)
	{
		m_indices.resize(m_indices.size() + numAddedIndices - numRemovedIndices);
	}

	void Renderer::recalcIndices(std::size_t start)
	{
		for (std::size_t i = start; i < m_drawables.size(); i++) {
			IndexInfo indexInfo = m_indexInfos[i];
			const Mesh& mesh = m_drawables[i]->mesh;
			const std::vector<GLushort>& indices = mesh.getIndices();
			
			for (std::size_t j = 0; j < indices.size(); j++)
				m_indices[indexInfo.position + j] = indices[j] + indexInfo.offset;
		}
	}

	void Renderer::print()
	{
		std::cout << "indices:" << std::endl;
		for (std::size_t i = 0; i < m_drawables.size(); i++) {
			std::size_t startPos = m_indexInfos[i].position;
			std::size_t numIndices = m_drawables[i]->mesh.getIndexCount();
			for (std::size_t j = 0; j < numIndices; j++)
				std::cout << m_indices[startPos + j] << " ";

			std::cout << "| ";
		}

		std::cout << std::endl;

		std::cout << "press any key to continue" << std::endl;
		std::cin.get();
	}
}