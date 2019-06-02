#include "Tween.h"
#include "Logger.h"
#include "Easing.h"

namespace sb
{
	Tween::EasingCommand Tween::getCurrentEasingCommand(float t) {
		for (std::size_t i = 0; i < _easingCommands.size(); i++) {
			if (t >= _easingCommands[i].t0 && t < _easingCommands[i].t1)
				return _easingCommands[i];
		}

		SB_ERROR("this line must never be reached");
		return EasingCommand();
	}

	float Tween::value(float t) {
		if (_easingCommands.empty())
			return 0;

		if (t < _easingCommands[0].t0)
			return _easingCommands[0].from;

		if (t >= _easingCommands[_easingCommands.size() - 1].t1)
			return _easingCommands[_easingCommands.size() - 1].to;

		EasingCommand easingCommand = getCurrentEasingCommand(t);
		return easingCommand.easing(t, easingCommand.t0, easingCommand.t1, easingCommand.from, easingCommand.to);
	}

	float Tween::getDuration() const {
		return _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
	}

	Tween& Tween::bounceOut(float from, float to, float duration) {
		addEasing<Easing::bounceOut>(from, to, duration);
		return *this;
	}

	Tween& Tween::quintInOut(float from, float to, float duration) {
		addEasing<Easing::quintInOut>(from, to, duration);
		return *this;
	}

	Tween& Tween::linear(float from, float to, float duration) {
		addEasing<Easing::linear>(from, to, duration);
		return *this;
	}

	Tween& Tween::wait(float value, float duration) {
		addEasing<Easing::linear>(value, value, duration);
		return *this;
	}

	Tween& Tween::wait(float duration) {
		float value = !_easingCommands.empty() ? _easingCommands[_easingCommands.size() - 1].to : 0;
		return wait(value, duration);
	}

	Tween Tween::None(float value) {
		return Tween().linear(value, value, 0);
	}
}

