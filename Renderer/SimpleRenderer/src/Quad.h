#pragma once

#include "Drawable.h"

namespace sb
{
	class Quad : public Drawable
	{
	public:
		Quad() : Drawable(QuadMesh) { }

	private:
		static const Mesh QuadMesh;
	};
}