#include "Drawable.h"
#include "Window.h"

namespace sb
{
	void Drawable::draw(Window& window)
	{
		window.draw(this);
	}

	void Drawable::setTransform(const Transform& transform)
	{
		m_transform = transform;
		refreshTransform();
	}

	void Drawable::refreshTransform()
	{
		const std::vector<Vertex>& vertices = m_mesh.getVertices();
		std::vector<Vector2f> positions(vertices.size());

		for (std::size_t i = 0; i < vertices.size(); i++)
			positions[i] = m_transform * vertices[i].position;

		m_transformedMesh.setPositions(positions);
	}
}