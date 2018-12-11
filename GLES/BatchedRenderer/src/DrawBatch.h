#pragma once

#include "Drawable.h"
#include <vector>
#include <utility> 

namespace sb
{
	class DrawBatch
	{
	public:
		virtual ~DrawBatch();

		// https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c
		template<typename T, typename... Args>
		T* create(Args&&... args)
		{
			T* drawable = new T(args ...);
			m_drawables.push_back(drawable);

			return drawable;
		}

	private:
		std::vector<Drawable*> m_drawables;
	};
}