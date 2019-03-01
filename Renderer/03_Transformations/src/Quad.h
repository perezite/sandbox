#pragma once

#include "Drawable.h"

namespace sb
{
	class Quad : public Drawable
	{
	public:
		virtual void draw(Window& window);

	private:
		static const Mesh QuadMesh;
	};
}