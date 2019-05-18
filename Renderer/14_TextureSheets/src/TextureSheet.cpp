#include "TextureSheet.h"
#include "Logger.h"

namespace sb
{
	TextureSheet::TextureSheet(int width, int height, bool allowOversize)
	{
		if (!allowOversize)
			checkSize(width, height);

		m_texture.createEmpty(width, height, sb::Color(0, 0, 0, 0));
	}

	void TextureSheet::loadFromAsset(const std::string& assetPath, const sb::Vector2i& targetBottomLeft, bool flipVertically)
	{
		m_texture.loadFromAssetIntoSubArea(assetPath, targetBottomLeft, flipVertically);
	}

	void TextureSheet::checkSize(int width, int height)
	{
		static int maxEntent = 1024;

		if (width > maxEntent || height > maxEntent)
			SB_ERROR("The specified texture sheet size is too large. The maximum allowed width/height is " << maxEntent << " pixels");
	}
}

