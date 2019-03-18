#include "Entity.h"

namespace sb
{
	Entity::~Entity()
	{
		for (std::size_t i = 0; i < m_components.size(); i++)
			delete m_components[i];
	}

	void Entity::addComponent(Component* component) {
		component->setParent(this);
		m_components.push_back(component);
	}

	void Entity::drawComponent(Component* component, DrawTarget& target, RenderStates states) 
	{
		states.transform *= getTransform();

		component->draw(target, states);
	}
}
