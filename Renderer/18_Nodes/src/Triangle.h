#pragma once

#include "Node.h"
#include "Transformable.h"

namespace sb
{
	class Triangle : public Node<Triangle>, public Transformable
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