#include "Quad.h"

namespace sb
{
	const Mesh Quad::QuadMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(0, 0, 1, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(0, 1, 1, 1))
	}, PrimitiveType::TriangleStrip);
}

