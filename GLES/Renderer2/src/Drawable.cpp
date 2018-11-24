#include "Drawable.h"

namespace sb 
{
	namespace renderer2
	{
		const Mesh Drawable::m_mesh({
			Vertex( Vector2f(-0.5f, -0.5f), Color(1, 0, 0, 1)),
			Vertex( Vector2f( 0.5f, -0.5f), Color(0, 1, 0, 1)),
			Vertex( Vector2f( 0.0f,  0.5f), Color(0, 0, 1, 1))
		});
	}
}