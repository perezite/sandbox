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
		: m_surface(NULL), m_handle(0), m_hasMipmaps(false), m_mipmapsGenerated(false)
		{ }

		~Texture();

		Texture(const std::string filePath, bool flipVertically = true);
		
		inline const bool hasMipmaps() const { return m_hasMipmaps; }

		inline const float* getTexTransform() const { return m_texTransform; }
		
		void loadFromAsset(const std::string& assetPath, bool flipVertically = true);

		void createEmpty(int width, int height, const Color& color = sb::Color(0, 0, 0, 0));

		void enableMipmaps(bool enable);

		void bind() const;

	protected:
		void createSurface(const std::string& assetPath, bool flipVertically);

		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurface(SDL_Surface* surface);

		void flipSurfacePixels(SDL_Surface* destination, SDL_Surface* target);

		void createEmptyTexture(int width, int height, const Color& color);

		void createGlTexture(int width, int height, void* pixels);

		void updateTexTransform(int visibleWidth, int visibleHeight, int interalWidth, int internalHeight);
		
		void activateMipmaps();

		void deactivateMipmaps();

	private:
		SDL_Surface* m_surface;

		GLuint m_handle;

		Vector2i m_visibleSize;

		Vector2i m_internalSize;

		bool m_hasMipmaps;

		bool m_mipmapsGenerated;

		float m_texTransform[9];
	};
}