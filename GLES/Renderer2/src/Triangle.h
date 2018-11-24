#pragma once

#include "Drawable.h"

namespace sb
{
	namespace renderer2
	{
		class Triangle : public Drawable
		{
		public:
			Triangle(const Vector2f& position, const Vector2f scale)
				: Drawable(position, scale)
			{ }

			inline const Mesh& getMesh() const { return TheMesh; };

		private:
			static const Mesh TheMesh;
		};
	}
}