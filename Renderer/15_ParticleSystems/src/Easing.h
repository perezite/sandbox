#pragma once
#include "Math.h"

namespace sb
{
	class Easing {
		typedef float(*computeFunction)(float t, float b, float c, float d);

	private:
		static inline float computeLinear(float t, float b, float c, float d) {
			return c * t / d + b;
		}

		static inline float computeSineIn(float t, float b, float c, float d) {
			return -c * cos(t / d * (sb::Pi / 2)) + c + b;
		}

		static inline float computeSineOut(float t, float b, float c, float d) {
			return c * sin(t / d * (sb::Pi / 2)) + b;
		}

		static inline float computeSineInOut(float t, float b, float c, float d) {
			return -c / 2 * (cos(sb::Pi * t / d) - 1) + b;
		}

		static inline float computeQuadIn(float t, float b, float c, float d) {
			return c*(t /= d)*t + b;

		}

		static inline float computeQuadOut(float t, float b, float c, float d) {
			return -c *(t /= d)*(t - 2) + b;
		}

		static inline float computeQuadInOut(float t, float b, float c, float d) {
			if ((t /= d / 2) < 1) return ((c / 2)*(t*t)) + b;
			return -c / 2 * (((t - 2)*(--t)) - 1) + b;
		}

		static inline float computeCubicIn(float t, float b, float c, float d) {
			return c*(t /= d)*t*t + b;
		}

		static inline float computeCubicOut(float t, float b, float c, float d) {
			return c*((t = t / d - 1)*t*t + 1) + b;
		}

		static inline float computeCubicInOut(float t, float b, float c, float d) {
			if ((t /= d / 2) < 1) return c / 2 * t*t*t + b;
			return c / 2 * ((t -= 2)*t*t + 2) + b;
		}

		static float computeQuintInOut(float t, float b, float c, float d) {
			if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
			t = t - 2;
			return c / 2 * (t*t*t*t*t + 2) + b;
		}

		static inline float computeBounceOut(float t, float b, float c, float d) {
			if ((t /= d) < (1 / 2.75f)) {
				return c * (7.5625f * t * t) + b;
			}
			else if (t < 2 / 2.75f) {
				float postFix = t -= (1.5f / 2.75f);
				return c * (7.5625f * postFix * t + .75f) + b;
			}
			else if (t < 2.5 / 2.75) {
				float postFix = t -= (2.25f / 2.75f);
				return c * (7.5625f * postFix * t + .9375f) + b;
			}
			else {
				float postFix = t -= (2.625f / 2.75f);
				return c * (7.5625f * postFix * t + .984375f) + b;
			}
		}

		template <computeFunction F>
		static inline float compute(float t, float t0, float t1, float from, float to) {
			float duration = t1 - t0;
			t = clamp(t, t0, t1);
			return F(t - t0, from, to - from, duration);
		}

	public:

		static inline float linear(float t, float t0, float t1, float from, float to) {
			return compute<computeLinear>(t, t0, t1, from, to);
		}

		static inline float sineIn(float t, float t0, float t1, float from, float to) {
			return compute<computeSineIn>(t, t0, t1, from, to);
		}

		static inline float sineOut(float t, float t0, float t1, float from, float to) {
			return compute<computeSineOut>(t, t0, t1, from, to);
		}

		static inline float sineInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeSineInOut>(t, t0, t1, from, to);
		}

		static inline float quadIn(float t, float t0, float t1, float from, float to) {
			return compute<computeQuadIn>(t, t0, t1, from, to);
		}

		static inline float quadOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuadOut>(t, t0, t1, from, to);
		}

		static inline float quadInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuadInOut>(t, t0, t1, from, to);
		}

		static inline float cubicIn(float t, float t0, float t1, float from, float to) {
			return compute<computeCubicIn>(t, t0, t1, from, to);
		}

		static inline float cubicOut(float t, float t0, float t1, float from, float to) {
			return compute<computeCubicOut>(t, t0, t1, from, to);
		}

		static inline float cubicInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeCubicInOut>(t, t0, t1, from, to);
		}

		static inline float quintInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuintInOut>(t, t0, t1, from, to);
		}

		static inline float bounceOut(float t, float t0, float t1, float from, float to) {
			return compute<computeBounceOut>(t, t0, t1, from, to);
		}

	};
}