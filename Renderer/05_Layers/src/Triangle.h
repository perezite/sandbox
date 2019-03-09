#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle() 
		{ }

		virtual void draw(DrawTarget& window, RenderStates states);

	private:
		static const Mesh TriangleMesh;
	};
}