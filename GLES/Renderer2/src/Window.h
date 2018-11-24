#pragma once

#ifdef  WIN32
	#include <gl/glew.h>
	#include <SDL2/SDL_opengl.h>
	#include <gl/glu.h>
#else
	#include <GLES2/gl2.h>	
	#include <GLES2/gl2ext.h>
#endif
#include <SDL2/SDL.h>

namespace sb
{
	namespace renderer2
	{
		class Window
		{
		public:
			bool hasQuitEvent() { return m_hasQuitEvent; }

			void init(int width = 400, int height = 400);

			void flip();

			void update();

			void destroy();

		private:
			bool m_hasQuitEvent;

			SDL_Window* m_sdlWindow;

			SDL_GLContext m_glContext;
		};
	}
}