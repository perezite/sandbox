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
			float t0 = t / d;
			return c * t0 * t0 + b;
		}

		static inline float computeQuadOut(float t, float b, float c, float d) {
			float t0 = t / d;
			return -c *(t0)*(t0 - 2) + b;
		}

		static inline float computeQuadInOut(float t, float b, float c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 1;
			if (t0 < 1) return ((c / 2)*(t0*t0)) + b;
			return -c / 2 * (((t1 - 2)*t1) - 1) + b;
		}

		static inline float computeCubicIn(float t, float b, float c, float d) {
			float t0 = t / d;
			return c*t0*t0*t0 + b;
		}

		static inline float computeCubicOut(float t, float b, float c, float d) {
			float t0 = t / d - 1;
			return c*(t0*t0*t0 + 1) + b;
		}

		static inline float computeCubicInOut(float t, float b, float c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 2;
			if (t0 < 1) return c / 2 * t0*t0*t0 + b;
			return c / 2 * (t1*t1*t1 + 2) + b;
		}

		static inline float computeQuartIn(float t, float b, float c, float d) {
			float t0 = t / d;
			return c*t0*t0*t0*t0 + b;
		}

		static inline float computeQuartOut(float t, float b, float c, float d) {
			float t0 = t / d - 1;
			return -c * (t0*t0*t0*t0 - 1) + b;
		}

		static inline float computeQuartInOut(float t, float b, float c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 2;
			if (t0 < 1) return c / 2 * t0*t0*t0*t0 + b;
			return -c / 2 * (t1*t1*t1*t1 - 2) + b;
		}

		static float computeQuintIn(float t, float b, float c, float d) {
			float t0 = t / d;
			return c*t0*t0*t0*t0*t0 + b;
		}

		static float computeQuintOut(float t, float b, float c, float d) {
			float t0 = t / d - 1;
			return c*(t0*t0*t0*t0*t0 + 1) + b;
		}

		static float computeQuintInOut(float t, float b, float c, float d) {
			float t0 = t / (d / 2);
			if (t0 < 1) return c / 2 * t0*t0*t0*t0*t0 + b;
			t0 = t0 - 2;
			return c / 2 * (t0*t0*t0*t0*t0 + 2) + b;
		}

		static float computeExpoIn(float t, float b, float c, float d) {
			return (t == 0) ? b : c * pow(2.0f, 10 * (t / d - 1)) + b;
		}

		static float computeExpoOut(float t, float b, float c, float d) {
			return (t == d) ? b + c : c * (-pow(2.0f, -10 * t / d) + 1) + b;
		}

		static float computeExpoInOut(float t, float b, float c, float d) {
			if (t == 0) return b;
			if (t == d) return b + c;
			if ((t /= d / 2) < 1) return c / 2 * pow(2.0f, 10 * (t - 1)) + b;
			return c / 2 * (-pow(2.0f, -10 * --t) + 2) + b;
		}

		static float computeCircIn(float t, float b, float c, float d) {
			float t0 = t / d;
			return -c * (sqrt(1 - t0*t0) - 1) + b;
		}

		static float computeCircOut(float t, float b, float c, float d) {
			float t0 = t / d - 1;
			return c * sqrt(1 - t0*t0) + b;
		}

		static float computeCircInOut(float t, float b, float c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 2;
			if (t0 < 1) return -c / 2 * (sqrt(1 - t0*t0) - 1) + b;
			return c / 2 * (sqrt(1 - t1*t1) + 1) + b;
		}

		static inline float computeBounceOut(float t, float b, float c, float d) {
			float t0 = t / d;
			if (t0 < (1 / 2.75f)) {
				return c * (7.5625f * t0 * t0) + b;
			}
			else if (t0 < 2 / 2.75f) {
				float postFix = t0 -= (1.5f / 2.75f);
				return c * (7.5625f * postFix * t0 + .75f) + b;
			}
			else if (t0 < 2.5 / 2.75) {
				float postFix = t0 -= (2.25f / 2.75f);
				return c * (7.5625f * postFix * t0 + .9375f) + b;
			}
			else {
				float postFix = t0 -= (2.625f / 2.75f);
				return c * (7.5625f * postFix * t0 + .984375f) + b;
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

		static inline float quartIn(float t, float t0, float t1, float from, float to) {
			return compute<computeQuartIn>(t, t0, t1, from, to);
		}

		static inline float quartOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuartOut>(t, t0, t1, from, to);
		}

		static inline float quartInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuartInOut>(t, t0, t1, from, to);
		}

		static inline float quintIn(float t, float t0, float t1, float from, float to) {
			return compute<computeQuintIn>(t, t0, t1, from, to);
		}


		static inline float quintOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuintOut>(t, t0, t1, from, to);
		}

		static inline float quintInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeQuintInOut>(t, t0, t1, from, to);
		}

		static inline float expoIn(float t, float t0, float t1, float from, float to) {
			return compute<computeExpoIn>(t, t0, t1, from, to);
		}

		static inline float expoOut(float t, float t0, float t1, float from, float to) {
			return compute<computeExpoOut>(t, t0, t1, from, to);
		}

		static inline float expoInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeExpoInOut>(t, t0, t1, from, to);
		}

		static inline float circIn(float t, float t0, float t1, float from, float to) {
			return compute<computeCircIn>(t, t0, t1, from, to);
		}


		static inline float circOut(float t, float t0, float t1, float from, float to) {
			return compute<computeCircOut>(t, t0, t1, from, to);
		}

		static inline float circInOut(float t, float t0, float t1, float from, float to) {
			return compute<computeCircInOut>(t, t0, t1, from, to);
		}

		static inline float bounceOut(float t, float t0, float t1, float from, float to) {
			return compute<computeBounceOut>(t, t0, t1, from, to);
		}

	};
}