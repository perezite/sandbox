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

	void Sprite::draw(DrawTarget& target, DrawStates states)
	{
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(SpriteMesh.getVertices(), SpriteMesh.getPrimitiveType(), states);
	}
}
