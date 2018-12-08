#include "Material.h"

namespace sb
{
	Material& Material::getDefault()
	{
		static Material defaultMaterial(Shader::getDefault());
		return defaultMaterial;	
	}

	bool operator <(Material& left, Material& right)
	{
		return *left.getShader() < *right.getShader();
	}

	bool operator ==(Material& left, Material& right)
	{
		return *left.getShader() == *right.getShader();
	}

}