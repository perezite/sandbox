#pragma once
#include "Transformable.h"
#include "Component.h"
#include "Transform.h"
#include "Window.h"
#include "RenderStates.h"
#include <vector>

namespace sb
{
	class Entity : public Transformable
	{
	public:
		~Entity();

		inline std::vector<Component*> getComponents() const { return m_components; }

		void addComponent(Component* component);

		void drawComponent(Component* component, DrawTarget& target, RenderStates states = RenderStates::Default);

	private:
		std::vector<Component*> m_components;
	};
}