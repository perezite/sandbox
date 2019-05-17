#pragma once
#include "SDL.h"
#include "GL.h"
#include "Color.h"
#include "Vector2.h"
#include <string>

namespace sb 
{
	class Texture 
	{
	public:
		Texture()
		: m_surface(NULL), m_handle(0), m_mipmapsEnabled(false), m_mipmapsGenerated(false)
		{ }

		~Texture();

		Texture(const std::string filePath, bool flipVertically = true);
		
		inline const bool areMipmapsEnabled() const { return m_mipmapsEnabled; }

		inline const float* getTexTransform() const { return m_texTransform; }
		
		void loadFromAsset(const std::string& assetPath, bool flipVertically = true);

		void createEmpty(int width, int height, const Color& color = sb::Color(0, 0, 0, 0));

		void enableMipmaps(bool enable);

		void bind() const;

	protected:
		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurfaceVertically(SDL_Surface* surface);

		void flipPixelsVertically(SDL_Surface* destination, SDL_Surface* target);

		void createGlTexture(int width, int height, void* pixels);

		void activateMipmaps();

		void deactivateMipmaps();

		int getNextPowerOfTwo(int number);

		void createEmptyTexture(int width, int height, const Color& color);

		void storeSize(int visibleWidth, int visibleHeight, int interalWidth, int internalHeight);

		void createSurface(const std::string& assetPath, bool flipVertically);

	private:
		SDL_Surface* m_surface;

		GLuint m_handle;

		Vector2i m_visibleSize;

		Vector2i m_internalSize;

		bool m_mipmapsEnabled;

		bool m_mipmapsGenerated;

		float m_texTransform[9];
	};
}