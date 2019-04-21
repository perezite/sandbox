#include "Input.h"
#include <SDL2/SDL.h>

namespace sb
{
	bool Input::m_hasQuitEvent = false;
	std::set<KeyCode> Input::m_keysDown;
	std::set<KeyCode> Input::m_keysGoingDown;

	void Input::update()
	{
		SDL_Event event;
		m_keysGoingDown.clear();

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
			}
		}
	}
}
