#include "Window.h"
#include "Input.h"

namespace sb 
{
	Window::Window(int width, int height) 
		: m_isOpen(true)
	{
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

		GL_CHECK(glDisable(GL_DEPTH_TEST));
		GL_CHECK(glClearColor(1, 1, 1, 1));
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

	void Window::clear()
	{
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Window::draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const Transform& transform)
	{
		m_renderer->render(vertices, primitiveType, transform);
	}

	void Window::display()
	{
		SDL_GL_SwapWindow(m_sdlWindow);
	}
}
