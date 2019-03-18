#pragma once

#include "Mesh.h"
#include "RenderStates.h"
#include "Transformable.h"

namespace sb
{
	class DrawTarget;

	class Drawable : public Transformable
	{

	public:
		virtual ~Drawable() { };

		virtual void draw(DrawTarget& target, RenderStates states) = 0;
	};
}