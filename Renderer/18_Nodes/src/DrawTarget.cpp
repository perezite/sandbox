#include "DrawTarget.h"
#include <iostream>

namespace sb
{
	void DrawTarget::draw(Drawable* drawable, const DrawState& state)
	{
		drawable->draw(*this, state);
	}

	void DrawTarget::draw(Drawable& drawable, const DrawState& state)
	{
		drawable.draw(*this, state);
	}
}