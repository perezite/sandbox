#include "ShapeDrawer.h"
#include "Triangle.h"
#include "Quad.h"
#include "Entity.h"

namespace sb
{
	ShapeDrawer::ShapeDrawer(ShapeDrawerStyle style)
		: m_drawable(NULL)
	{
		setStyle(style);
	}

	ShapeDrawer::~ShapeDrawer()
	{
		if (m_drawable)
			delete m_drawable;
	}

	void ShapeDrawer::draw(DrawTarget& target, RenderStates states)
	{
		if (m_drawable)
			m_drawable->draw(target, states);
	}

	void ShapeDrawer::setStyle(ShapeDrawerStyle style)
	{
		if (m_drawable)
			delete m_drawable;
		 
		m_drawable = style == ShapeDrawerStyle::Triangle ? 
			(Drawable*)new Triangle() : (Drawable*)new Quad();
	}
}
