#include "Texture.h"

namespace sb
{
	Texture::Texture(const std::string filePath, bool flipVertically)
		: Texture()
	{
		loadFromFile(filePath, flipVertically);
	}

	void Texture::loadFromFile(const std::string filePath, bool invertHorizontally)
	{


	}
}