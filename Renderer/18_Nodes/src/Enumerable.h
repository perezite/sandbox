#pragma once
#include <vector>

namespace sb {
	template <class T>
	class Enumerable : public std::vector<T> {
		typedef std::vector<T> self;

	public:
		Enumerable()
		{ }

		Enumerable(size_t capacity) : std::vector<T>(capacity)
		{ }

		inline void adjust_capacity() {
			auto thresholdSize = self::capacity() / 4;

			if (self:: size() == 0)
				std::vector<T>().swap(*this);
			else if (self::size() < thresholdSize) {
				std::vector<T> resized(*this);
				resized.reserve(self::capacity() / 2);
				resized.swap(*this);
			}
		}
	};
}
