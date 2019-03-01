#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle() : Drawable(TriangleMesh) { } 

	private:
		static const Mesh TriangleMesh;
	};
}