#include "Quad.h"
#include "Window.h"

namespace sb
{
	Mesh Quad::QuadMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1), Vector2f(0, 0)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(0, 1, 0, 1), Vector2f(1, 0)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(0, 0, 1, 1), Vector2f(0, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(0, 1, 1, 1), Vector2f(1, 1))
	}, PrimitiveType::TriangleStrip);


	void Quad::update(Scene& scene) {
		updateChildren(scene);
	}

	void Quad::draw(DrawTarget& target, DrawStates states)
	{
		states.transform *= getTransform();
		states.drawLayer = getDrawLayer();
		target.draw(QuadMesh, states);
		drawChildren(target, states);
	}
}
