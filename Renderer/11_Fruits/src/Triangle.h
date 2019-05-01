#pragma once

#include "Drawable.h"
#include "Transformable.h"

namespace sb
{
	class Triangle : public Drawable, public Transformable
	{
	public:
		Triangle() 
		{ }

		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());

	private:
		static const Mesh TriangleMesh;
	};
}