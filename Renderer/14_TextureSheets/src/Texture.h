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
		: m_handle(0), m_areMipmapsEnabled(false), m_areMipmapsGenerated(false)
		{ }

		~Texture();

		Texture(const std::string filePath, bool flipVertically = true);
		
		inline const bool areMipmapsEnabled() const { return m_areMipmapsEnabled; }

		inline const float* getTexTransform() const { return m_texTransform; }
		
		void loadFromAsset(const std::string& assetPath, bool flipVertically = true);
		
		// void loadFromAssetIntoSubArea(const std::string& assetPath, const sb::Vector2i& areaBottomLeft, bool flipVertically = true);

		void createEmpty(int width, int height, const Color& color = sb::Color(0, 0, 0, 0));

		void enableMipmaps(bool enable);

		void bind() const;

	protected:
		void createEmptyTexture(int width, int height, const Color& color);

		void createGlTexture(int width, int height, void* pixels);

		void updateTexTransform(int visibleWidth, int visibleHeight, int interalWidth, int internalHeight);
		
		void activateMipmaps();

		void deactivateMipmaps();

	private:
		GLuint m_handle;

		Vector2i m_visibleSize;

		Vector2i m_internalSize;

		bool m_areMipmapsEnabled;

		bool m_areMipmapsGenerated;

		float m_texTransform[9];
	};
}