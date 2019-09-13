#include "DrawTarget.h"
#include <iostream>

namespace sb
{
	void DrawTarget::draw(Drawable* drawable, const DrawStates& state)
	{
		drawable->draw(*this, state);
	}

	void DrawTarget::draw(Drawable& drawable, const DrawStates& state)
	{
		drawable.draw(*this, state);
	}
}