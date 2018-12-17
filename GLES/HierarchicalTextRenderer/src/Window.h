#pragma once

#include "GL.h"
#include "Drawable.h"
#include "DrawBatch.h"
#include "MeshList.h"
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

		void display();

		void draw(Drawable* drawable) { m_renderer->render(drawable); }

		void draw(Drawable& drawable) { draw(&drawable); }

		void draw(DrawBatch& batch) { m_renderer->render(batch); }

		void draw(MeshList& meshList) { m_renderer->render(meshList); }

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;
	};
}
