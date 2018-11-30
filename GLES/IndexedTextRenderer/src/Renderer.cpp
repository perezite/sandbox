#include "Renderer.h"
#include "Error.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>

namespace sb
{
	void Renderer::init()
	{ }

	void Renderer::add(Drawable* drawable)
	{
		m_indexList.add(drawable);
	}

	void Renderer::remove(Drawable* drawable)
	{
		m_indexList.remove(drawable);
	}

	void Renderer::render()
	{
		m_indexList.calculate();
		m_indexList.print();
		print();
 	}

	void Renderer::print()
	{
		const::std::vector<GLushort>& indices =  m_indexList.getIndices();

		std::cout << "Raw indices: ";
		for (std::size_t i = 0; i < indices.size(); i++)
			std::cout << indices[i] << " ";
		std::cout << std::endl;
	}
}