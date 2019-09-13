#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"

namespace sb
{
	struct DrawState
	{
		Transform transform = Transform::Identity;

		Transform textureTransform = Transform::Identity; 

		const Texture* texture = NULL;

		Shader* shader = NULL;

		size_t drawLayer;

		DrawState()
		{ }

		DrawState(const Transform& transform_)
			: transform(transform_)
		{ }

		DrawState(const Texture& texture_)
			: texture(&texture_)
		{ }

		DrawState(Shader& shader_)
			: shader(&shader_)
		{ }

		DrawState(const Transform& transform_, const Texture& texture_, Shader& shader_)
			: transform(Transform::Identity), texture(&texture_), shader(&shader_)
		{ }

		static DrawState& getDefault();
	};

	const bool operator ==(const DrawState& left, const DrawState& right);

	const bool canBatch(const DrawState& left, const DrawState& right);

	inline const bool operator !=(const DrawState& left, const DrawState& right) { return !(left == right); }
}
