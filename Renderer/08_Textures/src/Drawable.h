#pragma once

#include "Mesh.h"
#include "Transformable.h"

namespace sb
{
	class DrawTarget;

	class Drawable : public Transformable
	{

	public:
		virtual ~Drawable() { };

		virtual void draw(DrawTarget& target, Transform transform = Transform::Identity) = 0;
	};
}