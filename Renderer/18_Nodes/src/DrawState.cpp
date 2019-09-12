#include "DrawState.h"

namespace sb
{
	DrawState& DrawState::getDefault()
	{
		static DrawState defaultStates;
		return defaultStates;
	}

	const bool operator==(const DrawState& left, const DrawState& right)
	{
		return left.shader == right.shader && left.texture == right.texture;
	}

	const bool canBatch(const DrawState & left, const DrawState & right)
	{
		return left.shader == right.shader && left.texture == right.texture;
	}
}
