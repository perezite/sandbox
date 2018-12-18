#include "Transform.h"


namespace sb
{
	Transform::Transform()
	{
		m_matrix[0] = 1; m_matrix[1] = 0; m_matrix[2] = 0;
		m_matrix[3] = 0; m_matrix[4] = 1; m_matrix[5] = 0;
		m_matrix[6] = 0; m_matrix[7] = 0; m_matrix[8] = 1;
	}

	Transform::Transform(Vector2f position, Vector2f scale, float rotation)
	{
		m_matrix[0] = 1; m_matrix[1] = 0; m_matrix[2] = position.x;
		m_matrix[3] = 0; m_matrix[4] = 1; m_matrix[5] = position.y;
		m_matrix[6] = 0; m_matrix[7] = 0; m_matrix[8] = 1;
	}

	Vector2f Transform::transformPoint(const Vector2f& point) const
	{
		return Vector2f(m_matrix[0] * point.x + m_matrix[1] * point.y + m_matrix[2],
						m_matrix[3] * point.x + m_matrix[4] * point.y + m_matrix[5]);
	}

	Vector2f operator*(const Transform& transform, const Vector2f& point)
	{
		return transform.transformPoint(point);
	}


	//Vector2f operator*(const Transform & transform, Vector2f right)
	//{
	//	float sa = sinf(transform.rotation);
	//	float ca = cosf(transform.rotation);
	//	float sx = transform.scale.x * right.x;
	//	float sy = transform.scale.y * right.y;

	//	return Vector2f(
	//		(ca * sx - sa * sy) + transform.position.x,
	//		(sa * sx + ca * sy) + transform.position.y);
	//}
}