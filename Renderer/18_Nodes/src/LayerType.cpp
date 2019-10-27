#include "LayerType.h"

namespace sb {
	const bool operator<(const LayerType& left, const LayerType& right) {
		return std::tie(left.drawStates, left.primitiveType) < std::tie(right.drawStates, right.primitiveType);
	}
}
