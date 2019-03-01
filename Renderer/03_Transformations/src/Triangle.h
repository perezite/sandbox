#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		virtual void draw(Window& window);

	private:
		static const Mesh TriangleMesh;
	};
}