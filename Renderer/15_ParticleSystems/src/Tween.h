#pragma once
#include "Easing.h"
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
		EasingCommand getCurrentEasingCommand(float t) const;

		template <easingFunction F>
		inline Tween& addEasing(float from, float to, float duration) {
			float t0 = _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
			float t1 = t0 + duration;
			_easingCommands.push_back(EasingCommand(t0, t1, from, to, F));
			return *this;
		}

	public:
		Tween() { }

		float value(float t) const;

		float getDuration() const;

		static Tween None(float value = 0);

		Tween& wait(float duration);

		inline Tween& wait(float value, float duration) { return addEasing<Easing::linear>(value, value, duration); }

		inline Tween& linear(float from, float to, float duration) { return addEasing<Easing::linear>(from, to, duration); }

		inline Tween& sineIn(float from, float to, float duration) { return addEasing<Easing::sineIn>(from, to, duration); }

		inline Tween& sineOut(float from, float to, float duration) { return addEasing<Easing::sineOut>(from, to, duration); }
					  
		inline Tween& sineInOut(float from, float to, float duration) { return addEasing<Easing::sineInOut>(from, to, duration); }
					  
		inline Tween& quadIn(float from, float to, float duration) { return addEasing<Easing::quadIn>(from, to, duration); }
					  
		inline Tween& quadOut(float from, float to, float duration) { return addEasing<Easing::quadOut>(from, to, duration); }
 					  
		inline Tween& quadInOut(float from, float to, float duration) { return addEasing<Easing::quadInOut>(from, to, duration); }
					  
		inline Tween& cubicIn(float from, float to, float duration) { return addEasing<Easing::cubicIn>(from, to, duration); }
					  
		inline Tween& cubicOut(float from, float to, float duration) { return addEasing<Easing::cubicOut>(from, to, duration); }
					  
		inline Tween& cubicInOut(float from, float to, float duration) { return addEasing<Easing::cubicInOut>(from, to, duration); }
					  
		inline Tween& quintInOut(float from, float to, float duration) { return addEasing<Easing::quintInOut>(from, to, duration); }
					  
		inline Tween& bounceOut(float from, float to, float duration) { return addEasing<Easing::bounceOut>(from, to, duration); }
	};
}
