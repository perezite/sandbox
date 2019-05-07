#include "Window.h"
#include "Input.h"
#include "GL.h"
#include "SDL.h"
#include <SDL2/SDL_image.h>

namespace sb 
{
	Window::Window(int width, int height) 
		: m_isOpen(true), m_resolution((float)width, (float)height), m_aspect((float)width/(float)height), m_inverseAspect((float)height/(float)width)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) SB_ERROR() << SDL_GetError() << std::endl;

		int flags = IMG_INIT_PNG && IMG_INIT_JPG;
		int imgResult = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & flags;
		if (imgResult != flags) SB_ERROR() << IMG_GetError() << std::endl;

		#ifdef WIN32
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == 0);
			m_sdlWindow = SDL_CreateWindow("Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			SDL_CHECK(m_sdlWindow); 
			m_glContext = SDL_GL_CreateContext(m_sdlWindow);
			SDL_CHECK(m_glContext);
			GLenum glewError = glewInit();
		#elif defined(__ANDROID__)
			SDL_DisplayMode mode;
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) == 0);
			SDL_CHECK(SDL_GetDisplayMode(0, 0, &mode) == 0);
			m_sdlWindow = SDL_CreateWindow("Sandbox", 0, 0, mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
			SDL_CHECK(m_sdlWindow);
			m_glContext = SDL_GL_CreateContext(m_sdlWindow);
			SDL_CHECK(m_glContext);
		#endif

		// the renderer expects an opened opengl context, so make sure it is created in the proper place!
		m_renderer = new Renderer();		
		
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
		GL_CHECK(glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
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
