#include "Machina.h"

namespace sb
{
	Machina::Machina(Vector2f position, Vector2f scale, float rotation, Shader* shader)
		:	Drawable(SliderMesh, Transform(position, scale, rotation), shader), 
			m_leftPropeller(Vector2f(-0.5f, -0.0f), Vector2f(0.1f, 0.1f)),
			m_rightPropeller(Vector2f(0.5f, 0.0f), Vector2f(0.1f, 0.1f))
	{ 
	}

	const Mesh Machina::SliderMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(0, 1, 0, 1)),
		Vertex(Vector2f(0.5f, -0.5f), Color(1, 0, 0, 1)),
		Vertex(Vector2f(0.5f,  0.5f), Color(0, 0, 1, 1)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(0, 1, 1, 1)),
	});

	void Machina::print()
	{

	}
}

