#pragma once

#include "KeyCode.h"
#include <set>

namespace sb 
{
	class Input
	{
	public:
		static void update();

		inline static bool hasQuitEvent() { return m_hasQuitEvent; }

		inline static bool isKeyDown(const KeyCode key) { return m_keysDown.count(key) > 0; }
		
		inline static bool isKeyGoingDown(const KeyCode key) { return m_keysGoingDown.count(key) > 0; }

	private:
		static bool m_hasQuitEvent;

		static std::set<KeyCode> m_keysDown;

		static std::set<KeyCode> m_keysGoingDown;
	};
}