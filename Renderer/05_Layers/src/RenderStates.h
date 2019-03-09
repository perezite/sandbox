#pragma once
#include "Transform.h"

namespace sb 
{
	struct RenderStates
	{
		RenderStates(Transform& transform_)
			: transform(transform_), depth(0)
		{ }

		RenderStates(float depth_)
			: transform(Transform::Identity), depth(depth_)
		{ }

		RenderStates(Transform& transform_, float depth_)
			: transform(transform_), depth(depth_)
		{ }

		Transform transform;

		float depth;

		static RenderStates Default;
	};
}