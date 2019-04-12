#pragma once

#include "Drawable.h"

namespace sb
{
	class Quad : public Drawable
	{
	public:
		Quad() 
		{ }
		
		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::Default);

	private:
		static const Mesh QuadMesh;
	};
}