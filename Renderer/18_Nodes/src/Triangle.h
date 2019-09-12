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

		virtual void draw(DrawTarget& window, DrawState state = DrawState::getDefault());

		inline Mesh& getMesh() { return TriangleMesh; }

	private:
		static Mesh TriangleMesh;
	};
}