#pragma once

#include "Drawable.h"

namespace sb
{
	class Rectangle : public Drawable
	{
	public:
		Rectangle()
			: Drawable(RectangleMesh)
		{ }

		void draw(Window& window) override;

	public:
		static const Mesh RectangleMesh;
	};
}