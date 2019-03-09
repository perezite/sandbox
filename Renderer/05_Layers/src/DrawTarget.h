#pragma once
#include "Vertex.h"
#include "PrimitiveType.h"
#include "RenderStates.h"
#include <vector>

namespace sb
{
	class DrawTarget
	{
	public:
		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType primitiveType, const RenderStates& states = RenderStates::Default) = 0;		
	};
}