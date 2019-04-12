#pragma once
#include "Transform.h"
#include "Texture.h"

namespace sb
{
	struct DrawStates
	{
		Transform transform;

		const Texture* texture;

		DrawStates()
			: transform(Transform::Identity), texture(NULL)
		{ }

		DrawStates(const Transform& transform_)
			: transform(transform_), texture(NULL)
		{ }

		DrawStates(const Texture& texture_)
			: transform(Transform::Identity), texture(&texture_)
		{ }

		DrawStates(const Transform& transform_, const Texture& texture_)
			: transform(Transform::Identity), texture(&texture_)
		{ }

		static const DrawStates Default;
	};
}