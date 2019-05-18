#pragma once
#include "Texture.h"

namespace sb
{
	class TextureSheet 
	{
	public:
		TextureSheet(int width, int height, bool allowOversize = true);

		void loadFromAsset(const std::string& assetPath, const sb::Vector2i& targetBottomLeft = sb::Vector2i(0, 0), bool flipVertically = true);

	protected:
		void checkSize(int width, int height);

	private:
		Texture m_texture;
	};
}