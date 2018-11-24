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
			Drawable(const Vector2f& position, const Vector2f scale)
				: m_transform(position, scale)
			{ }

			virtual ~Drawable()
			{ }

		public:
			inline Transform& getTransform() { return m_transform; }	

			inline virtual const Mesh& getMesh() const = 0;

		private: 
			Transform m_transform;

			static const Mesh TheMesh;
		};
	}
}
