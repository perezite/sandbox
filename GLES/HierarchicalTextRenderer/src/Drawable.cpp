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
		m_transform.setPosition(position); 
		m_meshNeedsUpdate = true;
	}

	void Drawable::setScale(const Vector2f& scale)
	{
		m_transform.setScale(scale);
		m_meshNeedsUpdate = true;
	}

	void Drawable::setRotation(const float rotation)
	{
		m_transform.setRotation(rotation);
		m_meshNeedsUpdate = true;
	}

	void Drawable::computeTransformedMesh()
	{
		if (m_meshNeedsUpdate)
			m_transformedMesh = m_transform * m_mesh;

		m_meshNeedsUpdate = false;
	}
}