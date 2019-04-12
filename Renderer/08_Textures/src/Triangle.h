#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle() 
		{ }

		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::Default);

	private:
		static const Mesh TriangleMesh;
	};
}