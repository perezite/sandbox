#include "Input.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace sb
{
	bool Input::m_hasQuitEvent = false;
	std::set<KeyCode> Input::m_keysDown;
	std::set<KeyCode> Input::m_keysGoingDown;
	std::set<SDL_FingerID> Input::m_touchesDown;
	std::set<SDL_FingerID> Input::m_touchesGoingDown;
	sb::Vector2f Input::m_mousePosition;
	sb::Vector2f Input::m_fingerPosition;
	bool Input::m_insideWindow;

	void Input::update()
	{
		SDL_Event event;
		m_keysGoingDown.clear();
		m_touchesGoingDown.clear();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					m_hasQuitEvent = true;
					break;
				case SDL_KEYDOWN:
					m_keysDown.insert((KeyCode)event.key.keysym.sym);
					m_keysGoingDown.insert((KeyCode)event.key.keysym.sym);
					break;
				case SDL_KEYUP:
					m_keysDown.erase((KeyCode)event.key.keysym.sym);
					break;
				case SDL_FINGERDOWN:
					m_touchesDown.insert(event.tfinger.fingerId);
					m_touchesGoingDown.insert(event.tfinger.fingerId);
					break;
				case SDL_FINGERUP:
					m_touchesDown.erase(event.tfinger.fingerId);
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_touchesDown.insert(event.button.button);
					m_touchesGoingDown.insert(event.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					m_touchesDown.erase(event.button.button);
					break;
				case SDL_MOUSEMOTION:
					m_mousePosition = sb::Vector2f((float)event.motion.x, (float)event.motion.y);
					break;
				case SDL_FINGERMOTION:
					m_fingerPosition = sb::Vector2f(event.tfinger.x, event.tfinger.y);
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_ENTER:
							m_insideWindow = true;
							break;
						case SDL_WINDOWEVENT_LEAVE:
							m_insideWindow = false;
							m_touchesDown.clear();
							m_touchesGoingDown.clear();
							break;
					}

					break;
			}
		}
	}

	sb::Vector2f Input::getTouchPosition(const sb::Vector2f& windowResolution)
	{
		#ifdef WIN32
			return sb::Vector2f(m_mousePosition.x, -m_mousePosition.y + windowResolution.y);
		#elif defined(__ANDROID__)
			float y = -m_fingerPosition.y + 1;
			return sb::Vector2f(windowResolution.x * m_fingerPosition.x, windowResolution.y * y);
		#else	
			#error os not supported
		#endif
	}

	sb::Vector2f Input::fingerToPixelCoordinates(const sb::Vector2f & position)
	{
		return sb::Vector2f();
	}
}
