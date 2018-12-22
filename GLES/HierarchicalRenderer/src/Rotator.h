#pragma once

#include "Drawable.h"
#include "Triangle.h"
#include "Window.h"

namespace sb
{
	class Rotator : public Drawable
	{
	public:
		Rotator(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0, Shader* shader = Shader::getDefault());

		// override 
		void draw(Window& window);

	private:
		Triangle m_leftPropeller;

		Triangle m_rightPropeller;

		static const Mesh RotatorMesh;
	};
}