#pragma once

#include "Drawable.h"

namespace sb
{
	class Triangle : public Drawable
	{
	public:
		Triangle()
			: m_mesh(TriangleMesh)
		{ }

		virtual void draw(Window& window) override;

	public:
		static const Mesh TriangleMesh;

	private:
		Mesh m_mesh;
	};
}