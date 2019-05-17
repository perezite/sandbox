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
		GL_CHECK(glDeleteTextures(1, &m_handle));
		SDL_FreeSurface(m_surface);
	}

	void Texture::enableMipmaps(bool enable) {
		if (!mipmapsEnabled() && enable)
			activateMipmaps();
		else if (mipmapsEnabled() && !enable)
			deactivateMipmaps();
	}

	void Texture::loadFromAsset(const std::string& assetPath, bool flipVertically)
	{
		std::string filePath = Asset::getFilePath(assetPath);
		m_surface = IMG_Load(filePath.c_str());
		SB_ERROR_IF(m_surface == NULL, IMG_GetError());
		m_surface = convertPixelFormat(m_surface, SDL_PIXELFORMAT_ABGR8888);
		if (flipVertically)
			m_surface = flipSurfaceVertically(m_surface);

		createGlTexture(m_surface->w, m_surface->h, m_surface->pixels);
	}

	void Texture::createEmpty(int width, int height, const Color& color) {
		std::vector<GLubyte> pixels(4 * width * height);
		for (std::size_t i = 0; i < pixels.size(); i += 4) {
			pixels[i + 0] = int(color.r * 255);
			pixels[i + 1] = int(color.g * 255);
			pixels[i + 2] = int(color.b * 255);
			pixels[i + 3] = int(color.a * 255);
		}
		
		createGlTexture(width, height, pixels.data());
	}

	void Texture::bind() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
	}

	void Texture::activateMipmaps()
	{
		if (!m_mipmapsGenerated) {
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			m_mipmapsGenerated = true;
		}

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		m_mipmapsEnabled = true;
	}

	void Texture::deactivateMipmaps()
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		m_mipmapsEnabled = false;
	}

	SDL_Surface* Texture::convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat)
	{
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, SDL_SWSURFACE);
		SDL_CHECK(converted);
		SDL_FreeSurface(surface);
		return converted;
	}

	SDL_Surface* Texture::flipSurfaceVertically(SDL_Surface* surface)
	{
		SDL_Surface *flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		SB_ERROR_IF(flipped == NULL, SDL_GetError());

		if (SDL_MUSTLOCK(flipped)) SDL_CHECK(SDL_LockSurface(flipped) < 0);

		flipPixelsVertically(surface, flipped);

		if (SDL_MUSTLOCK(flipped)) SDL_UnlockSurface(flipped);

		SDL_FreeSurface(surface);
		return flipped;
	}

	void Texture::flipPixelsVertically(SDL_Surface* destination, SDL_Surface* target) 
	{
		for (int row = destination->h - 1; row >= 0; row--)
		{
			for (int col = 0; col < destination->w; col++)
			{
				size_t sourceOffset = row * destination->w + col;
				size_t sourceOffsetBytes = sourceOffset * destination->format->BytesPerPixel;
				size_t destOffset = (destination->h - row - 1) * destination->w + col;
				size_t destOffsetBytes = destOffset * destination->format->BytesPerPixel;

				memcpy((char*)target->pixels + destOffsetBytes, (char*)destination->pixels + sourceOffsetBytes, destination->format->BytesPerPixel);
			}
		}
	}

	void Texture::createGlTexture(int width, int height, void* pixels)
	{
		GL_CHECK(glGenTextures(1, &m_handle));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
	}
}