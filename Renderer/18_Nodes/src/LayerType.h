#pragma once
#include "DrawStates.h"
#include "PrimitiveType.h"

namespace sb {
	struct LayerType {
		DrawStates drawStates;

		PrimitiveType primitiveType;

		LayerType(const DrawStates& drawStates_, const PrimitiveType& primitiveType_)
			: drawStates(drawStates_), primitiveType(primitiveType_)
		{ }
	};

	const bool operator<(const LayerType& left, const LayerType& right);
}
