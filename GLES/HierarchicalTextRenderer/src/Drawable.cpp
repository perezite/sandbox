#include "Drawable.h"
#include <math.h>

namespace sb
{
	void Drawable::setPosition(const Vector2f& position)
	{ 
		m_position = position;
		m_transformNeedsUpdate = true;
	}

	void Drawable::setScale(const Vector2f& scale)
	{
		m_scale = scale;
		m_transformNeedsUpdate = true;
	}

	void Drawable::setRotation(const float angle)
	{
		m_rotation = angle;
		m_transformNeedsUpdate = true;
	}

	Mesh& Drawable::getTransformedMesh() 
	{
		if (m_transformNeedsUpdate)
			updateTransform();

		transformMesh();
		return m_transformedMesh;
	}

	void Drawable::updateTransform()
	{
		m_transform = Transform(m_position, m_scale, m_rotation);
		m_transformNeedsUpdate = false;
	}

	void Drawable::transformMesh()
	{
		Transform fullTransform = m_parent ? m_parent->getTransform() * m_transform : m_transform;

		m_transformedMesh = fullTransform * m_mesh;
	}

}