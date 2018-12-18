#include "Drawable.h"
#include <math.h>

namespace sb
{
	void Drawable::draw(Window& window)
	{
		computeTransformedMesh();
		window.draw(m_transformedMesh);
	}

	void Drawable::setPosition(const Vector2f& position)
	{
		m_position = position;
		m_transformNeedsUpdate = true;
		m_meshNeedsUpdate = true;
	}

	void Drawable::computeTransformedMesh()
	{
		if (m_meshNeedsUpdate) {
			const Transform& transform = getTransform();
			const std::vector<Vertex>& vertices = m_mesh.getVertices();
			std::vector<Vector2f> positions(vertices.size());

			for (std::size_t i = 0; i < vertices.size(); i++)
				positions[i] = transform * vertices[i].position;

			m_transformedMesh.setPositions(positions);
		}

		m_meshNeedsUpdate = false;
	}

	const Transform& Drawable::getTransform()
	{
		if (m_transformNeedsUpdate) {
			m_transform = Transform(m_position, Vector2f(0, 0), 0);
		}

		m_transformNeedsUpdate = false;
		return m_transform;
	}

}