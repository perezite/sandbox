#pragma once

#include "Shape.h"

namespace sb
{
	class Triangle : public Shape
	{
	public:
		Triangle() 
			: Shape(TriangleMesh)
		{ }

		virtual void draw(Window& window, Transform transform);

	private:
		static const Mesh TriangleMesh;
	};
}