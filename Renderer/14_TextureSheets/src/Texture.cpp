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
		Image image(assetPath, flipVertically);

		sb::Vector2i powerOfTwoSize(nextPowerOfTwo(image.getWidth()), 
			nextPowerOfTwo(image.getHeight()));
		createEmptyTexture(powerOfTwoSize.x, powerOfTwoSize.y, sb::Color(0, 0, 0, 0));
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), 
			image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels()));

		updateTexTransform(image.getWidth(), image.getHeight(), powerOfTwoSize.x, powerOfTwoSize.y);
	}

	void Texture::createEmpty(int width, int height, const Color& color) {
		sb::Vector2i powerOfTwoSize(nextPowerOfTwo(width), nextPowerOfTwo(height));
		createEmptyTexture(powerOfTwoSize.x, powerOfTwoSize.y, color);

		updateTexTransform(width, height, powerOfTwoSize.x, powerOfTwoSize.y);
	}

	void Texture::enableMipmaps(bool enable) {
		if (!areMipmapsEnabled() && enable)
			activateMipmaps();
		else if (areMipmapsEnabled() && !enable)
			deactivateMipmaps();
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
		float* m = m_texTransform;
		float sx = float(visibleWidth - 1) / float(internalWidth);
		float sy = float(visibleHeight - 1) / (float)(internalHeight);
		float tx = 1 / float(2 * internalWidth);
		float ty = 1 / float(2 * internalHeight);

		m[0] = sx;		m[3] = 0;		m[6] = tx;
		m[1] = 0;		m[4] = sy;		m[7] = ty;
		m[2] = 0;		m[5] = 0;		m[8] = 0;
	}

	void Texture::activateMipmaps()
	{
		if (!m_areMipmapsGenerated) {
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			m_areMipmapsGenerated = true;
		}

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		m_areMipmapsEnabled = true;
	}

	void Texture::deactivateMipmaps()
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		m_areMipmapsEnabled = false;
	}
}