#pragma once

#include "Drawable.h"
#include <vector>

namespace sb
{
	class DrawBatch
	{
	public:
		void add(Drawable& drawable);

		void remove(Drawable& drawable);

	private:
		std::vector<Drawable*> m_drawables;
	};
}