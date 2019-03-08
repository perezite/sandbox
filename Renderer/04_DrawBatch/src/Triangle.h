#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle() 
		{ }

		virtual void draw(DrawTarget& window, Transform transform);

	private:
		static const Mesh TriangleMesh;
	};
}