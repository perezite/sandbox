#pragma once
#include "Mesh.h"
#include "DrawState.h"

namespace sb
{
	class DrawTarget;

	class Drawable	
	{

	public:
		virtual ~Drawable() { };

		virtual void draw(DrawTarget& target, DrawState drawState) = 0;
	};
}