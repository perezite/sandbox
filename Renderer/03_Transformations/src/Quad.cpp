#include "Quad.h"
#include "Window.h"

namespace sb
{
	const Mesh Quad::QuadMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(0, 0, 1, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(0, 1, 1, 1))
	}, PrimitiveType::TriangleStrip);

	void Quad::draw(Window& window, Transform transform)
	{
		transform *= getTransform();
		window.draw(QuadMesh.getVertices(), QuadMesh.getPrimitiveType(), transform);
	}

}

