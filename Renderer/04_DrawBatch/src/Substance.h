#pragma once

#include "PrimitiveType.h"
#include "Shape.h"
#include <tuple>

namespace sb
{
	struct Substance
	{
		Substance(const Shape& shape)
			: primitiveType(shape.getPrimitiveType())
		{ }

		PrimitiveType primitiveType;
	};

	inline bool operator <(const Substance& left, const Substance& right) {
		return std::tie(left.primitiveType) < std::tie(right.primitiveType);
	}
}
