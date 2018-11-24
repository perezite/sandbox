#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle()
			: Drawable(TriangleMesh, m_transform)
		{

		}

	private:
		static const Mesh TriangleMesh;

		Transform m_transform;
	};
}