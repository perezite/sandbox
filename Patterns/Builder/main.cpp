#include <string>
#include <vector>
#include <iostream>
#include <sstream>

struct Vector2f {
	Vector2f(float x_ = 0, float y_ = 0)
		: x(x_), y(y_)
	{ }
	
	float x, y;
};

class Texture {
public:
	void load(const std::string& path) {
	}
};

class Scene;

class Component {
public:
	Component(const std::string& type) 
		: _type(type)	
	{ }
	inline const std::string& getType() const { return _type; }
	virtual void update(Scene& scene) { };
	virtual void draw(Scene& scene) { };
	inline const Vector2f& getPosition() { return _position; }
private:
	Vector2f _position;
	std::string _type;
};

class Paddle : public Component {
public:
	Paddle() 
		: Component("Paddle")
	{ }
	void update(Scene& scene) {
		std::cout << "Component::update()" << std::endl;
		std::cout << "Paddle::update()" << std::endl;
	}
};

class Entity {
private:
	std::string _name;
	std::vector<Component*> _components;
	Vector2f _position;	
public:
	Entity(const std::string& name) 
		: _name(name)
	{ }
	virtual ~Entity() {
		for(std::size_t i = 0; i < _components.size(); i++)
			delete _components[i];
		_components.clear();
	}
	void setPosition(const Vector2f& position) { 
		_position = position;
	}
	inline const std::string& getName() const { return _name; }
	inline const Vector2f& getPosition() const { return _position; }
	inline const std::vector<Component*>& getComponents() const { return _components; }
	void addComponent(Component* component) {
		_components.push_back(component);
	}
	void update(Scene& scene) {
		std::cout << "Entity::update()" << std::endl;
		for (std::size_t i = 0; i < _components.size(); i++) 
			_components[i]->update(scene);
	}
};

class Scene {
private: 
	std::vector<Entity*> _entities;	
public:
	virtual ~Scene() {
		for(std::size_t i = 0; i < _entities.size(); i++)
			delete _entities[i];
		_entities.clear();
	}
	const std::vector<Entity*> getEntities() const {
		return _entities;
	}
	void addEntity(Entity* entity) {
		_entities.push_back(entity);
	}
	void update() {
		std::cout << "Scene::update()" << std::endl;
		for (std::size_t i = 0; i < _entities.size(); i++)
			_entities[i]->update(*this);
	}
};

class SceneBuilder {
private:
	Scene& _scene;
	Entity* _entity;
	Component* _component;
protected:
	void assertEntity() {
		if (!_entity) {
			std::cout << "Builder error: No entity set" << std::endl;
			exit(1);
		}
	}	
public:
	SceneBuilder(Scene& scene) 
		: _scene(scene), _entity(NULL)
	{ }
	SceneBuilder& addEntity(const std::string& name) {
		_entity = new Entity(name);
		_scene.addEntity(_entity);		
		return *this;
	}
	SceneBuilder& withComponent(Component* component) {
		assertEntity();
		_entity->addComponent(component);
		return *this;
	}		
	SceneBuilder& withPosition(const Vector2f& position) {
		assertEntity();
		_entity->setPosition(position);
		return *this;
	}
	SceneBuilder& withPosition(float x, float y) {
		return withPosition(Vector2f(x, y));
	}
};

class ScenePrinter {
protected:
	const std::string indent(std::size_t indentation = 0) {
		return std::string(indentation, ' ');
	}
public: 
	ScenePrinter() 
	{ }
	void print(const Component& component) {
		std::cout << indent(2) << "Component (" << component.getType() << ")" << std::endl; 
	}
	void print(const Entity& entity) {
		auto position = entity.getPosition();
		std::cout << indent(1) << "Entity (" << entity.getName() << "):" << std::endl;
		std::cout << indent(2) << "Position: (" << position.x << "," << position.y << ")" << std::endl; 
		auto components = entity.getComponents();
		for (std::size_t i = 0; i < components.size(); i++) 
			print(*components[i]);
	}
	void print(Scene& scene) {
		std::cout << "Scene: " << std::endl;
		auto entities = scene.getEntities();
		for (std::size_t i = 0; i < entities.size(); i++)
			print(*entities[i]);
	}
};

void run() {
	Scene scene;
	
	Texture paddleTexture;
	Texture propulsionTexture;
	paddleTexture.load("paddle.png");
	propulsionTexture.load("propulsion.png");
	
	SceneBuilder(scene).addEntity("paddle")
		.withPosition(1, 1)
		.withComponent(new Paddle());
		// .withComponent(new SpriteDrawer(paddleTexture))
		// .withChildEntity("leftPropulsion")
			// .withPosition(-0.5f, -0.2f)
			// .withScale(0.1f, 0.1f)
			// .withComponent(new Propulsion(false))
			// .withComponent(new ParticleSystemDrawer(propulsionTexture))
				// .withNumParticles(200)
			// .withComponent(new PropulsionExtra());

	ScenePrinter().print(scene);			
	scene.update();
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	run();
	
    return 0;
}