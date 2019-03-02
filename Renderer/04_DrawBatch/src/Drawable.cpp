#include "Drawable.h"

namespace sb 
{
	const Transform& Drawable::getTransform()
	{
		if (m_transformNeedsUpdate)
			updateTransform();

		return m_transform;
	}

	void Drawable::setPosition(const Vector2f& position)
	{
		m_position = position;
		m_transformNeedsUpdate = true;
	}

	void Drawable::setScale(const sb::Vector2f& scale)
	{
		m_scale = scale;
		m_transformNeedsUpdate = true;
	}

	void Drawable::setRotation(float radians)
	{
		m_rotation = radians;
		m_transformNeedsUpdate = true;
	}

	void Drawable::updateTransform()
	{
		m_transform = Transform(m_position, m_scale, m_rotation);
		m_transformNeedsUpdate = false;
	}
}