#include "DrawStates.h"

namespace sb
{
	DrawStates& DrawStates::getDefault() {
		static DrawStates defaultStates;
		return defaultStates;
	}

	const bool operator==(const DrawStates& left, const DrawStates& right) {
		return std::tie(left.drawLayer, left.shader, left.texture) == std::tie(right.drawLayer, right.shader, right.texture);
	}

	const bool operator<(const DrawStates& left, const DrawStates& right) {
		return std::tie(left.drawLayer, left.shader) < std::tie(right.drawLayer, right.shader);
	}

	const bool canBatch(const DrawStates & left, const DrawStates & right) {
		return std::tie(left.shader, left.texture) == std::tie(right.shader, right.texture);
	}
}
