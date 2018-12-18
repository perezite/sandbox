#pragma once

#include "GL.h"
#include "Renderer.h"
#include "Mesh.h"
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

		void display();

		void draw(Mesh& mesh) { m_renderer->render(mesh); }

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;
	};
}
