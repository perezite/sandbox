#pragma once
#include "SDL.h"
#include "GL.h"
#include "Color.h"
#include "Vector2.h"
#include "Transform.h"
#include "IntRect.h"
#include <string>

namespace sb
{
	class Texture 
	{
	public:
		Texture()
		: m_handle(0), m_isMipmapEnabled(false), m_isMipmapGenerated(false)
		{ }

		~Texture();

		Texture(const std::string filePath, bool flipVertically = true);
		
		inline const bool isMipmapEnabled() const { return m_isMipmapEnabled; }
		
		inline const Transform& getTransform() const { return m_transform; }

		void loadFromAsset(const std::string& assetPath, bool flipVertically = true);
		
		void createEmpty(int width, int height, const Color& color = sb::Color(0, 0, 0, 0));
		
		void loadFromAssetIntoSubArea(const std::string& assetPath, const sb::Vector2i& subAreaBottomLeft, bool flipVertically = true);

		void enableMipmap(bool enable);

		void bind() const;

		void computeAreaTransform(const IntRect* area, Transform& result) const;

	protected:
		void createEmptyTexture(int width, int height, const Color& color);

		void createGlTexture(int width, int height, void* pixels);

		void updateTransform(int visibleWidth, int visibleHeight, int interalWidth, int internalHeight);
		
		void activateMipmap();

		void deactivateMipmap();

	private:
		GLuint m_handle;

		bool m_isMipmapEnabled;

		bool m_isMipmapGenerated;

		Vector2i m_visibleSize;

		Vector2i m_internalSize;

		Transform m_transform;
	};
}