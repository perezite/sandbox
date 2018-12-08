#pragma once

#include "Shader.h"

namespace sb
{
	class Material
	{
	public:
		Material(Shader& shader = Shader::getDefault()) 
			: m_shader(&shader)
		{ }

		inline Shader* getShader() { return m_shader; }

	public:
		static Material& getDefault();

	private:
		Shader* m_shader;
	};

	bool operator <(Material& left, Material& right);

	bool operator ==(Material& left, Material& right);
}