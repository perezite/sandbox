#pragma once
#include "Texture.h"

namespace sb
{
	class TextureSheet 
	{
	public:
		TextureSheet(const sb::Vector2i& size, bool allowOversize = true);

		void loadFromAsset(const std::string& assetPath, const sb::Vector2i& targetBottomLeft = sb::Vector2i(0, 0), bool flipVertically = true);

	protected:
		void checkSize(const sb::Vector2i& size);

	private:
		Texture m_texture;
	};
}