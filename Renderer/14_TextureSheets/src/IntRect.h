#pragma once

namespace sb 
{
	struct IntRect 
	{
		IntRect(int left_, int top_, int width_, int height_)
			: left(left_), top(top_), width(width_), height(height_)
		{ }

		int left; int top;
		int width; int height;
	};
}