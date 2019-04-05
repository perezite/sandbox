#include "DrawTarget.h"
#include <iostream>

namespace sb
{
	void DrawTarget::draw(Drawable* drawable, const Transform & transform)
	{
		drawable->draw(*this, transform);
	}

	void DrawTarget::draw(Drawable& drawable, const Transform & transform)
	{
		drawable.draw(*this, transform);
	}
}

