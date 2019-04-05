#pragma once 
#include "Scene.h"
#include <string>

namespace sb 
{
	class SceneBuilder
	{
	public:
		SceneBuilder(Scene& scene) 
			: m_scene(scene)
		{ }

		SceneBuilder& addEntity(std::string name = "");

	private: 
		Scene& m_scene;
	};
}