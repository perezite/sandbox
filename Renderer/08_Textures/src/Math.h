#include <math.h>
#include "Vector2f.h"
#define _USE_MATH_DEFINES

namespace sb
{
	const float Pi = (float)M_PI;

	inline float random() {
		return float(rand()) / float(RAND_MAX);
	}

	inline float random(float min, float max) {
		return min + (max - min) * random();
	}

	inline float random(float max) {
		return random(0, max);
	}

	inline sb::Vector2f random2D(float min, float max) {
		return sb::Vector2f(random(min, max), random(min, max));
	}
}
