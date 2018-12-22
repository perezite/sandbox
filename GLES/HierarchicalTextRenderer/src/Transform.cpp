#include "Transform.h"
#include <math.h>

namespace sb
{
	Transform::Transform()
		: m_needsRefresh(false), m_position(0, 0), m_scale(1, 1), m_rotation(0)
	{
		m_matrix[0] = 1; m_matrix[1] = 0; m_matrix[2] = 0;
		m_matrix[3] = 0; m_matrix[4] = 1; m_matrix[5] = 0;
		m_matrix[6] = 0; m_matrix[7] = 0; m_matrix[8] = 1;
	}

	Transform::Transform(Vector2f position, Vector2f scale, float rotation)
		: m_needsRefresh(true), m_position(0, 0), m_scale(1, 1), m_rotation(0)
	{
		refresh();
	}

	void Transform::setPosition(const Vector2f& position)
	{
		m_position = position;
		m_needsRefresh = true;
	}

	void Transform::setScale(const Vector2f& scale)
	{
		m_scale = scale;
		m_needsRefresh = true;
	}

	void Transform::setRotation(const float rotation)
	{
		m_rotation = rotation;
		m_needsRefresh = true;
	}

	Vector2f Transform::getTransformedPoint(const Vector2f& point) const
	{
		if (m_needsRefresh)
			refresh();	

		return Vector2f(m_matrix[0] * point.x + m_matrix[1] * point.y + m_matrix[2],
						m_matrix[3] * point.x + m_matrix[4] * point.y + m_matrix[5]);
	}

	Mesh Transform::getTransformedMesh(const Mesh& mesh) const
	{
		if (m_needsRefresh)
			refresh();

		Mesh newMesh = mesh;
		const std::vector<Vertex>& vertices = mesh.getVertices();
		std::vector<Vector2f> newPositions(vertices.size());
		for (std::size_t i = 0; i < vertices.size(); i++)
			newPositions[i] = *this * vertices[i].position;

		newMesh.setPositions(newPositions);
		return newMesh;
	}

	void Transform::refresh() const
	{
		float c = cosf(m_rotation);
		float s = sinf(m_rotation);

		m_matrix[0] = c * m_scale.x;	m_matrix[1] = -s * m_scale.y;	m_matrix[2] = m_position.x;
		m_matrix[3] = s * m_scale.x;	m_matrix[4] = c * m_scale.y;	m_matrix[5] = m_position.y;
		m_matrix[6] = 0;				m_matrix[7] = 0;				m_matrix[8] = 1;

		m_needsRefresh = false;
	}

	Vector2f operator*(const Transform& transform, const Vector2f& point)
	{
		return transform.getTransformedPoint(point);
	}

	Mesh operator*(const Transform& transform, const Mesh& mesh)
	{
		return transform.getTransformedMesh(mesh);
	}
}