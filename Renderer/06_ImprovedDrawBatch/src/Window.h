#pragma once

#include "GL.h"
#include "Drawable.h"
#include "Renderer.h"
#include "DrawTarget.h"
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

		using DrawTarget::draw;
		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, const Transform& transform = Transform::Identity);

		void display();

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;
	};
}