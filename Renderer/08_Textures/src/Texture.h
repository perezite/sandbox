#pragma once
#include <SDL2/SDL.h>
#include "GL.h"
#include <string>

namespace sb 
{
	class Texture 
	{
	public:
		Texture()
			: m_surface(NULL), m_handle(0)
		{ }

		Texture(const std::string filePath, bool flipVertically = false);

		void loadFromFile(const std::string filePath, bool invertHorizontally = false);

	private:
		SDL_Surface* m_surface;

		GLuint m_handle;
	};
}