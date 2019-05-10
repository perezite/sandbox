#include "Camera.h"

namespace sb 
{
	Transform& Camera::getTransform()
	{
		if (m_transformNeedsUpdate)
			updateTransform();

		return m_transform;
	}

	void Camera::setPosition(const sb::Vector2f & position)
	{
		m_position = position;
		m_transformNeedsUpdate = true;
	}

	void Camera::setWidth(float width)
	{
		m_width = width;
		m_transformNeedsUpdate = true;
	}

	void Camera::setRotation(float rotation)
	{
		m_rotation = rotation;
		m_transformNeedsUpdate = true;
	}

	void Camera::updateTransform()
	{
		float* m = m_transform.getMatrix();
		float c = cosf(-m_rotation);
		float s = sinf(-m_rotation);

		sb::Vector2f inverseScale(2 / m_width, m_aspectRatio * 2 / m_width);

		m[0] = c * inverseScale.x;	m[3] = -s * inverseScale.x;		m[6] = -m_position.x;
		m[1] = s * inverseScale.y;	m[4] = c * inverseScale.y;		m[7] = -m_position.y;
		m[2] = 0;					m[5] = 0;						m[8] = 1;
	}
}