#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle()
			: Drawable(TriangleMesh)
		{ }

		void draw(Window& window) override;

	public:
		static const Mesh TriangleMesh;
	};
}