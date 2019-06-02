#pragma once
#include <vector>

namespace sb
{
	class Tween
	{
		typedef float(*easingFunction)(float t, float t0, float t1, float from, float to);

		struct EasingCommand {
			float t0;
			float t1;
			float from;
			float to;
			easingFunction easing;

			EasingCommand() { }

			EasingCommand(float t0_, float t1_, float from_, float to_, easingFunction easing_)
				: t0(t0_), t1(t1_), from(from_), to(to_), easing(easing_)
			{ }
		};

		std::vector<EasingCommand> _easingCommands;

	protected:
		EasingCommand getCurrentEasingCommand(float t);

		template <easingFunction F>
		inline void addEasing(float from, float to, float duration) {
			float t0 = _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
			float t1 = t0 + duration;
			_easingCommands.push_back(EasingCommand(t0, t1, from, to, F));
		}

	public:
		Tween() { }

		float value(float t);

		float getDuration() const;

		Tween& bounceOut(float from, float to, float duration);

		Tween& quintInOut(float from, float to, float duration);

		Tween& linear(float from, float to, float duration);

		Tween& wait(float value, float duration);

		Tween& wait(float duration);

		static Tween None(float value = 0);
	};
}
