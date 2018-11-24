#pragma once

namespace sb
{
	namespace renderer2
	{
		struct Vector2f
		{
			Vector2f(float x_ = 0, float y_ = 0) 
				: x(x_), y(y_)
			{ }

			float x, y;
		};
	}
}
