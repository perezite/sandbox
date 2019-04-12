#include "Texture.h"
#include "Asset.h"
#include <SDL2/SDL_image.h>

namespace sb
{
	Texture::Texture(const std::string filePath, bool flipVertically)
		: Texture()
	{
		loadFromAsset(filePath, flipVertically);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_handle);
		SDL_FreeSurface(m_surface);
	}

	void Texture::loadFromAsset(const std::string assetPath, bool flipVertically)
	{
		// create SDL surface
		std::string filePath = Asset::getFilePath(assetPath);
		m_surface = IMG_Load(filePath.c_str());
		SB_ERROR_IF(m_surface == NULL) << IMG_GetError() << std::endl;
		m_surface = convertPixelFormat(m_surface, SDL_PIXELFORMAT_ABGR8888);
		if (flipVertically)
			m_surface = flipSurfaceVertically(m_surface);

		// create opengl texture
		GL_CHECK(glGenTextures(1, &m_handle));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_surface->w, m_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_surface->pixels));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	}

	SDL_Surface* Texture::convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat)
	{
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, SDL_SWSURFACE);
		SB_ERROR_IF(converted == NULL) << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return converted;
	}

	SDL_Surface* Texture::flipSurfaceVertically(SDL_Surface* surface)
	{
		SDL_Surface *flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		SB_ERROR_IF(flipped == NULL) << SDL_GetError() << std::endl;

		// lock
		if (SDL_MUSTLOCK(flipped)) {
			int result = SDL_LockSurface(flipped);
			SB_ERROR_IF(result < 0) << SDL_GetError() << std::endl;
		}

		// flip
		for (int row = surface->h - 1; row >= 0; row--)
		{
			for (int col = 0; col < surface->w; col++)
			{
				size_t sourceOffset = row * surface->w + col;
				size_t sourceOffsetBytes = sourceOffset * surface->format->BytesPerPixel;
				size_t destOffset = (surface->h - row - 1) * surface->w + col;
				size_t destOffsetBytes = destOffset * surface->format->BytesPerPixel;

				memcpy((char*)flipped->pixels + destOffsetBytes, (char*)surface->pixels + sourceOffsetBytes, surface->format->BytesPerPixel);
			}
		}

		// unlock
		if (SDL_MUSTLOCK(flipped)) 
			SDL_UnlockSurface(flipped);

		SDL_FreeSurface(surface);
		return flipped;
	}
}