#pragma once

#include "Drawable.h"

namespace sb
{
	class Quad : public Drawable
	{
	public:
		virtual void draw(Window& window, Transform transform);

	private:
		static const Mesh QuadMesh;
	};
}