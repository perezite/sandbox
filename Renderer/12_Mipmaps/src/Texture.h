#pragma once
#include "SDL.h"
#include "GL.h"
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

		void loadFromAsset(const std::string filePath, bool flipVertically = true);

		void bind() const;

		void enableMipmaps(bool enable);

	private:
		void enableMipmaps();

		void disableMipmaps();

	protected:
		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurfaceVertically(SDL_Surface* surface);
		
	private:
		SDL_Surface* m_surface;

		GLuint m_handle;

		bool m_mipmapsEnabled;

		bool m_mipmapsGenerated;
	};
}