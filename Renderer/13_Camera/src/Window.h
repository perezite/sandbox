#pragma once

#include "Drawable.h"
#include "Renderer.h"
#include "DrawTarget.h"
#include "Camera.h"

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

		inline bool isOpen() { return m_isOpen; }

		inline Camera& getCamera() { return m_camera; }

		void update();

		void clear(const Color& clearColor = Color(0, 0, 0, 1));
		
		using DrawTarget::draw;
		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, 
			const DrawStates& states = DrawStates::getDefault());

		void display();

	protected:
		void computeCameraTransform(Transform& result, const sb::Vector2f& position, const sb::Vector2f& scale, float rotation);

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;

		Vector2f m_resolution;

		float m_aspect;

		float m_inverseAspect;

		Camera m_camera;
	};
}