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

		inline const sb::Vector2f& getResolution() const { return m_resolution; }

		inline const float getAspect() const { return m_aspect; }

		inline const float getInverseAspect() const { return m_inverseAspect; }

		bool isOpen() { return m_isOpen; }

		void update();

		void clear(const Color& clearColor = Color(0, 0, 0, 1));
		
		using DrawTarget::draw;
		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, 
			const DrawStates& states = DrawStates::getDefault());

		void display();

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;

		Vector2f m_resolution;

		float m_aspect;

		float m_inverseAspect;
	};
}