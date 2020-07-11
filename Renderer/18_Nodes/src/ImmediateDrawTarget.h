#pragma once
#include "DrawTarget.h"
#include <vector>

namespace sb
{
	class ImmediateDrawTarget : public DrawTarget
	{
	public:
		virtual void drawImmediate(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states) = 0;
	};
}