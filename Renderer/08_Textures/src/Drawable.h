#pragma once
#include "Mesh.h"
#include "Transformable.h"
#include "DrawStates.h"

namespace sb
{
	class DrawTarget;

	class Drawable : public Transformable
	{

	public:
		virtual ~Drawable() { };

		virtual void draw(DrawTarget& target, DrawStates drawStates) = 0;
	};
}