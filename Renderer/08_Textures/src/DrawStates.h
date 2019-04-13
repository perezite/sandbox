#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"

namespace sb
{
	struct DrawStates
	{
		Transform transform;

		const Texture* texture;

		Shader* shader;

		DrawStates()
			: transform(Transform::Identity), texture(NULL), shader(NULL)
		{ }

		DrawStates(const Transform& transform_)
			: transform(transform_), texture(NULL), shader(NULL)
		{ }

		DrawStates(const Texture& texture_)
			: transform(Transform::Identity), texture(&texture_), shader(NULL)
		{ }

		DrawStates(Shader& shader_)
			: transform(Transform::Identity), texture(NULL), shader(&shader_)
		{ }

		DrawStates(const Transform& transform_, const Texture& texture_, Shader& shader_)
			: transform(Transform::Identity), texture(&texture_), shader(&shader_)
		{ }

		static DrawStates& getDefault();
	};
}