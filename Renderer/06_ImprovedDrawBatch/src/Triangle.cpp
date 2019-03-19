#include "Triangle.h"
#include "Window.h"

namespace sb
{
	const Mesh Triangle::TriangleMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1)),
		Vertex(Vector2f(0.5f, -0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f(0.0f,  0.5f), Color(0, 0, 1, 1))
	}, PrimitiveType::Triangles);

	void Triangle::draw(DrawTarget& target, Transform transform)
	{
		transform *= getTransform();

		target.draw(TriangleMesh.getVertices(), TriangleMesh.getPrimitiveType(), transform);
	}
}
