#pragma once

#include "Shader.h"

namespace sb
{
	class Material
	{
	public:
		Material(Shader* shader_)
			: shader(shader_)
		{ }

		Shader* shader;
	};

	bool operator <(const Material& left, const Material& right);
}