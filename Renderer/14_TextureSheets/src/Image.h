#pragma once
#include "SDL.h"
#include <string>

namespace sb
{
	class Image
	{
	public:
		Image(const std::string& assetPath, bool flipVertically = true);

		virtual ~Image();

		inline void* getPixels() { return m_surface->pixels;  }

		inline int getWidth() { return m_surface->w; }

		inline int getHeight() { return m_surface->h; }

	protected:
		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurface(SDL_Surface* surface);

		void flipSurfacePixels(SDL_Surface* destination, SDL_Surface* target);

	private:
		SDL_Surface* m_surface;
	};
}