#pragma once

#include "Drawable.h"

namespace sb
{
	class Quad : public Drawable
	{
	public:
		Quad() 
		{ }
		
		virtual void draw(DrawTarget& window, Transform transform = Transform::Identity);

	private:
		static const Mesh QuadMesh;
	};
}