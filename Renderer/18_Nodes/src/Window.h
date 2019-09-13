#pragma once

#include "Drawable.h"
#include "Renderer.h"
#include "ImmediateDrawTarget.h"
#include "Camera.h"

namespace sb
{
	class Window : public ImmediateDrawTarget
	{
	public:
		Window(sb::Vector2i resolution) : Window(resolution.x, resolution.y)
		{ }

		Window(int width = 400, int height = 400);

		virtual ~Window();

		inline const sb::Vector2f& getResolution() const { return m_resolution; }

		inline const float getAspect() const { return m_camera.getAspectRatio(); }

		inline const float getInverseAspect() const { return m_camera.getInverseAspectRatio(); }

		inline bool isOpen() { return m_isOpen; }

		inline const Camera& getCamera() const { return m_camera; }

		inline Camera& getCamera() { return m_camera; }

		void update();

		void clear(const Color& clearColor = Color(0, 0, 0, 1));
		
		using DrawTarget::draw;
		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, 
			const DrawStates& state = DrawStates::getDefault());

		virtual void draw(const Mesh& mesh, const DrawStates& state = DrawStates::getDefault());

		virtual void drawImmediate(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, 
			const DrawStates& state = DrawStates::getDefault());

		void display();

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;

		Vector2f m_resolution;

		Camera m_camera;
	};
}