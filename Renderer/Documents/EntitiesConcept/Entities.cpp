void main() {
	Window window;
	Scene scene;
	SceneBuilder builder(scene);
	
	builder.addEntity("paddle")
		.withPosition(1, 1)
		.withRotation(30)
		.withComponent(new SpriteDrawer("mainPaddle.png"))	// variadic templates would be nice here ...
		.withComponent(new SpriteDrawer("mainPaddle.png", 
			drawLayer: 2, Vector2f(-0.5f, -0.5f)));
		
	while(window.isOpen()) {
		window.update();
		scene.update();
		window.draw(scene);
		// scene.draw(window);
		
		window.display();
	}
}

EntityBuilder& EntityBuilder::addEntity(std::string name) {
	Entity entity(name);
	m_entities.push_back(name);
	return *this;
}

EntityBuilder& EntityBuilder::withComponent(Component& component) {
	m_entity.addComponent(compoent);
	return *this;
}

EntityBuilder& EntityBuilder::withComponent(BatchComponent& batchComponent) {
	m_entity.addBatchComponent(batchComponent);
	return *this;
}

Scene::update() {
	for (auto entity: m_entities)
		entity.update();
	
	for (auto entity: m_batchEntities) 
		entity.update();
}

class Scene {
	class Sorting {
		unsigned int layer;
		unsigned int orderInLayer;
	}
}

Scene::draw(DrawTarget& target, RenderStates& states) {	
	computeWorldTransforms();
	
	DrawBatch batch;	
	for(auto component: components) {
		if (component->isBatched()) {
			batch.draw(component);
		} else {
			target.draw(batch);
			target.draw(component);
		}
	}
}

std::vector<Component*> Scene::collectAndSortComponents(std::vector<Entity*> entities) {
	std::vector<Component*> result;
	
	for (auto entity: m_entities) {
		components = m_entities.getComponents();
		std::insert(result.end(), components.begin(), components.end());
	}
	
	std::sort(result.begin(), result.end());
}

Scene::computeWorldTransforms() {
	for (std::size_t i = 0; i < m_entites.size(); i++)
		m_worldTransforms[m_entities] = computeWorldTransform(m_entities[i]);
}

Scene::computeWorldTransform(Entity& entity) {
	Entity* current = entity;
	Transform worldTransform = Transform::Identity;
	
	do {
		worldTransform *= current->getTransform();
	} while(current->getParent())
		
	m_worldTransforms[entity] = worldTransform;
}

class Component {
	virtual void update() { };
	
	virtual void draw() { };
	
	virtual bool isBatched() { return false; }
	
	virtual Material& getBatchMaterial() { return Material::Default; }
	
	virtual PrimitiveType getBatchPrimitiveType() { return PrimitiveType::Triangles; }
	
	bool operator<(Component* left, Component* right) {
		return std::tie(left->layer, left->orderInLayer, left->isBatched(), left->getBatchMaterial(), left->getBatchPrimitiveType()) <
			std::tie(right->layer, right->orderInLayer, right->isBatched(), right->getBatchMaterial(), right->getBatchPrimitiveType());
	}
}




