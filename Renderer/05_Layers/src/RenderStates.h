#pragma once
#include "Transform.h"

namespace sb 
{
	struct RenderStates
	{
		RenderStates(Transform& transform_)
			: transform(transform_)
		{ }

		Transform transform;

		static RenderStates Default;
	};
}