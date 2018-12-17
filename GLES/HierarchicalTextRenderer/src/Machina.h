#pragma once

#include "Drawable.h"
#include "Triangle.h"
#include "Window.h"

namespace sb
{
	class Machina : public Drawable
	{
	public:
		Machina(Vector2f position = Vector2f(0, 0), Vector2f scale = Vector2f(1, 1), float rotation = 0, Shader* shader = Shader::getDefault());

		void print();

	private:
		Triangle m_leftPropeller;

		Triangle m_rightPropeller;

		static const Mesh SliderMesh;
	};
}