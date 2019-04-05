#pragma once
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Transform.h"
#include "Drawable.h"
#include <vector>

namespace sb
{
	class DrawTarget
	{
	public:
		void draw(Drawable* drawable, const Transform& transform = Transform::Identity);

		void draw(Drawable& drawable, const Transform& transform = Transform::Identity);

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const Transform& transform = Transform::Identity) = 0;		
	};
}