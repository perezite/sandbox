#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Renderer.h"
#include "DrawTarget.h"
#include "RenderStates.h"
#include <SDL2/SDL.h>

namespace sb
{
	class Window : public DrawTarget
	{
	public:
		Window(int width = 400, int height = 400);

		~Window();

		bool isOpen() { return m_isOpen; }

		void update();

		void clear();

		void draw(Drawable* drawable, const RenderStates& states);

		inline void draw(Drawable& drawable, const RenderStates& states = RenderStates::Default) { draw(&drawable, states); }

		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType, const RenderStates& states = RenderStates::Default);

		void display();

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;
	};
}