#include "Rectangle.h"

namespace sb
{
	const Mesh Rectangle::RectangleMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(0, 0, 1, 1)),
	});
}

