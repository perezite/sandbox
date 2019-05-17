#pragma once
#include "SDL.h"
#include "GL.h"
#include "Color.h"
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
		
		inline const bool mipmapsEnabled() const { return m_mipmapsEnabled; }
		
		void enableMipmaps(bool enable);

		// void loadFromAsset(const std::string& assetPath, const Vector2i& offset = Vector2i(0, 0), bool flipVertically = true);
		void loadFromAsset(const std::string& assetPath, bool flipVertically = true);

		void createEmpty(int width, int height, const Color& color);

		void bind() const;

	protected:
		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurfaceVertically(SDL_Surface* surface);

		void flipPixelsVertically(SDL_Surface* destination, SDL_Surface* target);

		void activateMipmaps();

		void deactivateMipmaps();

		void createGlTexture(int width, int height, void* pixels);
	private:
		SDL_Surface* m_surface;

		GLuint m_handle;

		bool m_mipmapsEnabled;

		bool m_mipmapsGenerated;
	};
}