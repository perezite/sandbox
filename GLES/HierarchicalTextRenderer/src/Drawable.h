#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Window.h"

namespace sb
{
	class Drawable
	{
	public:
		Drawable()
		{ }

		virtual void draw(Window& window) = 0;

	private:

		Transform m_transform;
	};
}