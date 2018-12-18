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

	public:
		static const Mesh TriangleMesh;
	};
}