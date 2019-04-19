#pragma once

#include "Drawable.h"

namespace sb
{
	class Quad : public Drawable
	{
	public:
		Quad() 
		{ }
		
		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());

		inline Mesh& getMesh() { return QuadMesh; }

	private:
		static Mesh QuadMesh;
	};
}