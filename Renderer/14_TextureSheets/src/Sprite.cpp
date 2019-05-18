#include "Sprite.h"
#include "DrawTarget.h"

namespace sb
{
	Mesh Sprite::SpriteMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 1, 1, 1), Vector2f(0, 0)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(1, 1, 1, 1), Vector2f(1, 0)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(1, 1, 1, 1), Vector2f(0, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(1, 1, 1, 1), Vector2f(1, 1))
	}, PrimitiveType::TriangleStrip);

	void Sprite::setTexture(Texture& texture)
	{
		m_texture = &texture;
		m_hasCustomTextureTransform = false;
	}

	void Sprite::setTexture(Texture& texture, const IntRect& textureArea)
	{
		m_texture = &texture; 
		texture.computeTransform(textureArea, m_textureTransform);
		m_hasCustomTextureTransform = true;
	}

	void Sprite::draw(DrawTarget& target, DrawStates states)
	{
		states.transform *= getTransform();
		states.texture = m_texture;
		states.customTextureTransform = m_textureTransform;
		states.hasCustomTextureTransform = m_hasCustomTextureTransform;
		target.draw(SpriteMesh.getVertices(), SpriteMesh.getPrimitiveType(), states);
	}
}
