#pragma once

#include "Vector2f.h"
#include "Color.h"

namespace sb
{
	namespace renderer2
	{
		struct Vertex
		{
			Vertex() { }

			Vertex(const Vector2f& position_, const Color& color_)
				: position(position_), color(color_)
			{ }

			Vector2f position;
			Color color;
		};
	}
}