#pragma once

#include "Shape.h"

namespace sb
{
	class Quad : public Shape
	{
	public:
		Quad() 
			: Shape(QuadMesh)
		{ }

		virtual void draw(Window& window, Transform transform);

	private:
		static const Mesh QuadMesh;
	};
}