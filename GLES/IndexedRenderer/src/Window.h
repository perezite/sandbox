#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

namespace sb
{
	class Window
	{
	public:
		Window(int width = 400, int height = 400);

		~Window();

		bool isOpen() { return m_isOpen; }

		void update();

		void draw();

		void show(Drawable* drawable) { m_renderer.add(drawable); }

		void hide(Drawable* drawable) { m_renderer.remove(drawable); }

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer m_renderer;
	};
}