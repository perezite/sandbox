#pragma once

#include "Node.h"

namespace sb
{
	class Quad : public Node {
	public:
		Quad() 
		{ }
		
		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());

		inline Mesh& getMesh() { return QuadMesh; }
	private:
		static Mesh QuadMesh;
	};
}