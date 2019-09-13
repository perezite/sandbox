#pragma once
#include "Vertex.h"
#include "PrimitiveType.h"
#include "DrawStates.h"
#include "Drawable.h"
#include <vector>

namespace sb
{
	class DrawTarget
	{
	public:
		void draw(Drawable* drawable, const DrawStates& state = DrawStates::getDefault());

		void draw(Drawable& drawable, const DrawStates& state = DrawStates::getDefault());

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& state = DrawStates::getDefault()) = 0;	

		virtual void draw(const Mesh& mesh, const DrawStates &state = DrawStates::getDefault()) = 0;
	};
}