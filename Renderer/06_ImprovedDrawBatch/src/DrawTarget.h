#pragma once
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Transform.h"
#include <vector>

namespace sb
{
	class DrawTarget
	{
	public:
		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const Transform& transform = Transform::Identity) = 0;		
	};
}