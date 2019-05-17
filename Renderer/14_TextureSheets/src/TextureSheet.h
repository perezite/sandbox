#pragma once
#include "Texture.h"

namespace sb
{
	class TextureSheet 
	{
	public:
		// the default maximum allowed size is 1024 x 1024
		// This setting can be overridden by setting allowOversize = true
		TextureSheet(int width, int height, bool allowOversize = true);

	protected:
		void checkSize(int width, int height);

	private:
		Texture m_texture;
	};
}