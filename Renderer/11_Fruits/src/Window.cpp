#include "Window.h"
#include "Input.h"
#include "GL.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace sb 
{
	Window::Window(int width, int height) 
		: m_isOpen(true), m_resolution(width, height)
	{
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

		#ifdef WIN32
			SDL_Init(SDL_INIT_VIDEO);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			m_sdlWindow = SDL_CreateWindow("Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			m_glContext = SDL_GL_CreateContext(m_sdlWindow);
			GLenum glewError = glewInit();
		#elif defined(__ANDROID__)
			SDL_Init(SDL_INIT_VIDEO);
			SDL_DisplayMode mode;
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GetDisplayMode(0, 0, &mode);
			m_sdlWindow = SDL_CreateWindow("Sandbox", 0, 0, mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
			m_glContext = SDL_GL_CreateContext(m_sdlWindow);
		#endif

		// the renderer expects an opened opengl context, so make sure it is created in the proper place!
		m_renderer = new Renderer();		
		
		//auto bla = SDL_GL_GetSwapInterval();
		//auto blub = SDL_GL_SetSwapInterval(0);
		

		GL_CHECK(glDisable(GL_DEPTH_TEST));
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_sdlWindow);
		SDL_Quit();
	}

	void Window::update()
	{
		if (Input::hasQuitEvent())
			m_isOpen = false;
	}

	void Window::clear(const Color& clearColor)
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Window::draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states)
	{
		m_renderer->render(vertices, primitiveType, states);
	}

	void Window::display()
	{
		SDL_GL_SwapWindow(m_sdlWindow);
	}
}
