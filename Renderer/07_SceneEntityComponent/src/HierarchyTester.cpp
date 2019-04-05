#include "HierarchyTester.h"

namespace sb
{
	HierarchyTester::HierarchyTester()
	{
		m_quad.setScale(2, 1);
		m_leftTriangle.setScale(0.5f, 0.5f);
		m_leftTriangle.setPosition(-1, -0.25f);
		m_rightTriangle.setScale(0.5f, 0.5f);
		m_rightTriangle.setPosition(1, -0.25f);
	}

	void HierarchyTester::draw(DrawTarget & target, Transform transform)
	{
		transform *= getTransform();

		m_quad.draw(target, transform);
		m_leftTriangle.draw(target, transform);
		m_rightTriangle.draw(target, transform);
	}
}
