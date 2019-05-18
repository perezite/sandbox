#include "TextureSheet.h"
#include "Logger.h"

namespace sb
{
	TextureSheet::TextureSheet(const sb::Vector2i& size, bool allowOversize)
	{
		if (!allowOversize)
			checkSize(size);

		m_texture.createEmpty(size, sb::Color(0, 0, 0, 0));
	}

	void TextureSheet::loadFromAsset(const std::string& assetPath, const sb::Vector2i& targetBottomLeft, bool flipVertically)
	{
		m_texture.loadFromAssetIntoSubArea(assetPath, targetBottomLeft, flipVertically);
	}

	void TextureSheet::checkSize(const sb::Vector2i& size)
	{
		static int maxEntent = 1024;

		if (size.x > maxEntent || size.y > maxEntent)
			SB_ERROR("The specified texture sheet size is too large. The maximum allowed width/height is " << maxEntent << " pixels");
	}
}

