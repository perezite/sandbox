#pragma once

namespace sb
{
	class Easing {
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

		static float computeQuintInOut(float t, float b, float c, float d) {
			if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
			t = t - 2;
			return c / 2 * (t*t*t*t*t + 2) + b;
		}

		static inline float computeLinear(float t, float b, float c, float d) {
			return c * t / d + b;
		}

	public:
		static inline float bounceOut(float t, float t0, float t1, float from, float to) {
			float duration = t1 - t0;
			float time = t < t0 ? t0 : t > t1 ? t1 : t;
			return computeBounceOut(time - t0, from, to - from, duration);
		}

		static inline float quintInOut(float t, float t0, float t1, float from, float to) {
			float duration = t1 - t0;
			float time = t < t0 ? t0 : t > t1 ? t1 : t;
			return computeQuintInOut(time - t0, from, to - from, duration);
		}

		static inline float linear(float t, float t0, float t1, float from, float to) {
			float duration = t1 - t0;
			float time = t < t0 ? t0 : t > t1 ? t1 : t;
			return computeLinear(time - t0, from, to - from, duration);
		}
	};
}