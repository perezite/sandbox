#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <windows.h>

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

class Component {
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
	const std::string& getName() const { return _name; }
	const Vector2f& getPosition() const { return _position; }
	void setPosition(const Vector2f& position) { 
		_position = position;
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
};

class SceneBuilder {
private:
	Scene& _scene;
	Entity* _entity;
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
	void print(Entity& entity) {
		auto position = entity.getPosition();
		std::cout << indent(1) << "Entity (" << entity.getName() << "):" << std::endl;
		std::cout << indent(2) << "Position: (" << position.x << "," << position.y << ")" << std::endl; 
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
		.withPosition(1, 1);
	// scene.addEntity("paddle")
		// .withPosition(1, 1)
		// .withComponent(new Paddle())
		// .withComponent(new SpriteDrawer(paddleTexture))
		// .withChildEntity("leftPropulsion")
			// .withPosition(-0.5f, -0.2f)
			// .withScale(0.1f, 0.1f)
			// .withComponent(new Propulsion(false))
			// .withComponent(new ParticleSystemDrawer(propulsionTexture))
				// .withNumParticles(200)
			// .withComponent(new PropulsionExtra());
			
	ScenePrinter().print(scene);
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	run();
	
    return 0;
}