#pragma once
#include "Drawable.h"
#include "DrawTarget.h"
#include "Transform.h"
#include "Quad.h"
#include "Triangle.h"

namespace sb
{
	class HierarchyTester : public Drawable
	{
	public:
		HierarchyTester();

		virtual void draw(DrawTarget& target, Transform transform);

	private: 
		Quad m_quad;

		Triangle m_leftTriangle;

		Triangle m_rightTriangle;
	};
}