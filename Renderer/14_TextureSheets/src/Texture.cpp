#include "Texture.h"
#include "Asset.h"
#include "Math.h"
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

	void Texture::loadFromAsset(const std::string& assetPath, bool flipVertically)
	{
		createSurface(assetPath, flipVertically);

		sb::Vector2i powerOfTwoSize(nextPowerOfTwo(m_surface->w), nextPowerOfTwo(m_surface->h));
		createEmptyTexture(powerOfTwoSize.x, powerOfTwoSize.y, sb::Color(0, 0, 0, 0));
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_surface->w, m_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, m_surface->pixels));

		updateTexTransform(m_surface->w, m_surface->h, powerOfTwoSize.x, powerOfTwoSize.y);
	}

	void Texture::createEmpty(int width, int height, const Color& color) {
		sb::Vector2i powerOfTwoSize(nextPowerOfTwo(width), nextPowerOfTwo(height));
		createEmptyTexture(powerOfTwoSize.x, powerOfTwoSize.y, color);

		updateTexTransform(width, height, powerOfTwoSize.x, powerOfTwoSize.y);
	}

	void Texture::enableMipmaps(bool enable) {
		if (!hasMipmaps() && enable)
			activateMipmaps();
		else if (hasMipmaps() && !enable)
			deactivateMipmaps();
	}

	void Texture::bind() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
	}

	void Texture::createSurface(const std::string& assetPath, bool flipVertically)
	{
		std::string filePath = Asset::getFilePath(assetPath);
		m_surface = IMG_Load(filePath.c_str());
		SB_ERROR_IF(m_surface == NULL, IMG_GetError());
		m_surface = convertPixelFormat(m_surface, SDL_PIXELFORMAT_ABGR8888);
		if (flipVertically)
			m_surface = flipSurface(m_surface);
	}

	SDL_Surface* Texture::convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat)
	{
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, SDL_SWSURFACE);
		SDL_CHECK(converted);
		SDL_FreeSurface(surface);
		return converted;
	}

	SDL_Surface* Texture::flipSurface(SDL_Surface* surface)
	{
		SDL_Surface *flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		SB_ERROR_IF(flipped == NULL, SDL_GetError());

		if (SDL_MUSTLOCK(flipped)) SDL_CHECK(SDL_LockSurface(flipped) < 0);

		flipSurfacePixels(surface, flipped);

		if (SDL_MUSTLOCK(flipped)) SDL_UnlockSurface(flipped);

		SDL_FreeSurface(surface);
		return flipped;
	}

	void Texture::flipSurfacePixels(SDL_Surface* destination, SDL_Surface* target)
	{
		for (int row = destination->h - 1; row >= 0; row--) {
			for (int col = 0; col < destination->w; col++) {
				size_t sourceOffset = row * destination->w + col;
				size_t sourceOffsetBytes = sourceOffset * destination->format->BytesPerPixel;
				size_t destOffset = (destination->h - row - 1) * destination->w + col;
				size_t destOffsetBytes = destOffset * destination->format->BytesPerPixel;

				memcpy((char*)target->pixels + destOffsetBytes, (char*)destination->pixels + sourceOffsetBytes, destination->format->BytesPerPixel);
			}
		}
	}

	void Texture::createEmptyTexture(int width, int height, const Color& color)
	{
		std::vector<GLubyte> pixels(4 * width * height);
		for (std::size_t i = 0; i < pixels.size(); i += 4) {
			pixels[i + 0] = int(color.r * 255);
			pixels[i + 1] = int(color.g * 255);
			pixels[i + 2] = int(color.b * 255);
			pixels[i + 3] = int(color.a * 255);
		}

		createGlTexture(width, height, pixels.data());
	}

	void Texture::createGlTexture(int width, int height, void* pixels)
	{
		GL_CHECK(glGenTextures(1, &m_handle));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
	}

	void Texture::updateTexTransform(int visibleWidth, int visibleHeight, int internalWidth, int internalHeight)
	{
		float sx = float(visibleWidth - 1) / float(internalWidth);
		float sy = float(visibleHeight - 1) / (float)(internalHeight);
		float tx = 1 / float(2 * internalWidth);
		float ty = 1 / float(2 * internalHeight);

		float* m = m_texTransform;
		m[0] = sx;			m[3] = 0;			m[6] = tx;
		m[1] = 0;			m[4] = sy;			m[7] = ty;
		m[2] = 0;			m[5] = 0;			m[8] = 0;
	}

	void Texture::activateMipmaps()
	{
		if (!m_mipmapsGenerated) {
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			m_mipmapsGenerated = true;
		}

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		m_hasMipmaps = true;
	}

	void Texture::deactivateMipmaps()
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		m_hasMipmaps = false;
	}
}