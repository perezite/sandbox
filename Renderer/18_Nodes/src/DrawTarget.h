#pragma once
#include "Vertex.h"
#include "PrimitiveType.h"
#include "DrawState.h"
#include "Drawable.h"
#include <vector>

namespace sb
{
	class DrawTarget
	{
	public:
		void draw(Drawable* drawable, const DrawState& state = DrawState::getDefault());

		void draw(Drawable& drawable, const DrawState& state = DrawState::getDefault());

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawState& state = DrawState::getDefault()) = 0;	

		virtual void draw(const Mesh& mesh, const DrawState &state = DrawState::getDefault()) = 0;
	};
}