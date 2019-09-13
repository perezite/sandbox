#pragma once

#include "Node.h"

namespace sb
{
	class Triangle : public Node
	{
	public:
		Triangle() 
		{ }

		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());

		inline Mesh& getMesh() { return TriangleMesh; }

	private:
		static Mesh TriangleMesh;
	};
}