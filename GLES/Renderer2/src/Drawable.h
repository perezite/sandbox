#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace sb
{
	namespace renderer2
	{
		class Drawable
		{
		public:
			Drawable()
			{ }

			Drawable(const Vector2f& position, const Vector2f scale)
				: m_transform(position, scale)
			{ }

		public:
			inline Transform& getTransform() { return m_transform; }
			
			inline const Mesh& getMesh() const { return m_mesh; }

		private: 
			Transform m_transform;

			static const Mesh m_mesh;
		};
	}
}
