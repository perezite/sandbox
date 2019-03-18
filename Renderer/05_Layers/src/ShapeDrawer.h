#pragma once
#include "Component.h"
#include "Drawable.h"

namespace sb
{
	enum class ShapeDrawerStyle {
		Triangle, Quad
	};

	class ShapeDrawer : public Component 
	{
	public:
		ShapeDrawer(ShapeDrawerStyle style);

		~ShapeDrawer();

		virtual void draw(DrawTarget& target, RenderStates states);

		void setStyle(ShapeDrawerStyle style);

	private:
		ShapeDrawerStyle m_style;

		Drawable* m_drawable;
	};
}