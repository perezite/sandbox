#pragma once

#include "Shader.h"

namespace sb
{
	class Material
	{
	public:
		Shader shader;
	};

	bool operator <(const Material& left, const Material& right);
}