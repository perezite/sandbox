#include "Texture.h"
#include "Asset.h"
#include "Math.h"
#include "Image.h"

namespace sb
{
	Texture::Texture(const std::string filePath, bool flipVertically)
		: Texture()
	{
		loadFromAsset(filePath, flipVertically);
	}

	Texture::~Texture() {
		GL_CHECK(glDeleteTextures(1, &m_handle));
	}

	void Texture::loadFromAsset(const std::string& assetPath, bool flipVertically)
	{
		SB_ERROR_IF(m_handle != 0, "The texture has already been initialized");

		Image image(assetPath, flipVertically);

		sb::Vector2i powerOfTwoSize(nextPowerOfTwo(image.getWidth()), 
			nextPowerOfTwo(image.getHeight()));
		createEmptyTexture(powerOfTwoSize.x, powerOfTwoSize.y, sb::Color(0, 0, 0, 0));
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), 
			image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels()));

		updateTexTransform(image.getWidth(), image.getHeight(), powerOfTwoSize.x, powerOfTwoSize.y);
	}

	void Texture::createEmpty(int width, int height, const Color& color) {
		SB_ERROR_IF(m_handle != 0, "The texture has already been initialized");

		sb::Vector2i powerOfTwoSize(nextPowerOfTwo(width), nextPowerOfTwo(height));
		createEmptyTexture(powerOfTwoSize.x, powerOfTwoSize.y, color);

		updateTexTransform(width, height, powerOfTwoSize.x, powerOfTwoSize.y);
	}

	void Texture::loadFromAssetIntoSubArea(const std::string & assetPath, const sb::Vector2i & bottomLeft, bool flipVertically)
	{
		SB_ERROR_IF(m_handle == 0, "This function can only be called on an already initialized texture");

		Image image(assetPath, flipVertically);

		bool outsideVisibleArea = bottomLeft.x + image.getWidth() > m_visibleSize.x ||
			bottomLeft.y + image.getHeight() > m_visibleSize.y;
		SB_ERROR_IF(outsideVisibleArea, "The existing texture is too small for the loaded image at the given position");

		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, bottomLeft.x, bottomLeft.y, image.getWidth(),
			image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels()));
	}

	void Texture::enableMipmap(bool enable) {
		if (!isMipmapEnabled() && enable)
			activateMipmap();
		else if (isMipmapEnabled() && !enable)
			deactivateMipmap();
	}

	void Texture::bind() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
	}

	void Texture::createEmptyTexture(int width, int height, const Color& color)
	{
		std::vector<GLubyte> pixels(4 * width * height);
		for (std::size_t i = 0; i < pixels.size(); i += 4) {
			pixels[i + 0] = int(color.r * 255);
			pixels[i + 1] = int(color.g * 255);
			pixels[i + 2] = int(color.b * 255);
			pixels[i + 3] = int(color.a * 255);
		}

		createGlTexture(width, height, pixels.data());
	}

	void Texture::createGlTexture(int width, int height, void* pixels)
	{
		GL_CHECK(glGenTextures(1, &m_handle));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
	}

	void Texture::updateTexTransform(int visibleWidth, int visibleHeight, int internalWidth, int internalHeight)
	{
		m_visibleSize = sb::Vector2i(visibleWidth, visibleHeight);

		float* m = m_texTransform;
		float sx = float(visibleWidth - 1) / float(internalWidth);
		float sy = float(visibleHeight - 1) / (float)(internalHeight);
		float tx = 1 / float(2 * internalWidth);
		float ty = 1 / float(2 * internalHeight);

		m[0] = sx;		m[3] = 0;		m[6] = tx;
		m[1] = 0;		m[4] = sy;		m[7] = ty;
		m[2] = 0;		m[5] = 0;		m[8] = 0;
	}

	void Texture::activateMipmap()
	{
		if (!m_isMipmapGenerated) {
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			m_isMipmapGenerated = true;
		}

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		m_isMipmapEnabled = true;
	}

	void Texture::deactivateMipmap()
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		m_isMipmapEnabled = false;
	}
}