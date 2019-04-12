#pragma once

#include "Drawable.h"
#include "Renderer.h"
#include "DrawTarget.h"

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
		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, 
			const DrawStates& states = DrawStates::Default);

		void display();

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;
	};
}