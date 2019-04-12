#include "DrawStates.h"

namespace sb
{
	DrawStates& DrawStates::getDefault()
	{
		static DrawStates defaultStates;
		return defaultStates;
	}
}
