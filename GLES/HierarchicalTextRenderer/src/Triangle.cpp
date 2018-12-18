#include "Triangle.h"
#include "Window.h"

namespace sb
{
	void Triangle::draw(Window& window)
	{
		window.draw(m_mesh);
	}

	const Mesh Triangle::TriangleMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1)),
		Vertex(Vector2f(0.5f, -0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f(0.0f,  0.5f), Color(0, 0, 1, 1))
	});
}

