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

		~Texture();

		Texture(const std::string filePath, bool flipVertically = true);

		void loadFromAsset(const std::string filePath, bool flipVertically = true);

		void bind() const;

	protected:
		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurfaceVertically(SDL_Surface* surface);

	private:
		SDL_Surface* m_surface;

		GLuint m_handle;
	};
}