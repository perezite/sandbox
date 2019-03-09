#include "Triangle.h"
#include "Window.h"

namespace sb
{
	const Mesh Triangle::TriangleMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 0.5f)),
		Vertex(Vector2f(0.5f, -0.5f), Color(0, 1, 0, 0.5f)),
		Vertex(Vector2f(0.0f,  0.5f), Color(0, 0, 1, 0.5f))
	}, PrimitiveType::Triangles);

	void Triangle::draw(DrawTarget& target, RenderStates states)
	{
		states.transform *= getTransform();

		target.draw(TriangleMesh.getVertices(), TriangleMesh.getPrimitiveType(), states);
	}
}

