#pragma once

#include "Drawable.h"
#include "Transformable.h"

namespace sb
{
	class Quad : public Drawable, public Transformable
	{
	public:
		Quad() 
		{ }
		
		virtual void draw(DrawTarget& window, DrawState state = DrawState::getDefault());

		inline Mesh& getMesh() { return QuadMesh; }

	private:
		static Mesh QuadMesh;
	};
}