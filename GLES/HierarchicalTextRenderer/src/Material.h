#pragma once

#include "Shader.h"

namespace sb
{
	class Material
	{
	public:
		Material()

		{ }

		Shader* shader;

		int layer;
	};

	bool operator <(const Material& left, const Material& right);
}