#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <windows.h>

class Printer {
public: 
	Printer() 
		: _tabs(0) 
	{ }
	std::ostream& print() {
		for (std::size_t i = 0; i < _tabs; i++)
			_stream << " ";
		return _stream;
	}
	void addIndent(std::size_t numTabs) {
		_tabs += numTabs;
	}
	std::ostringstream& getStream() {
		return _stream;
	}
private:
	std::ostringstream _stream;
	std::size_t _tabs;
};

struct Vector2f {
	Vector2f(float x_ = 0, float y_ = 0)
		: x(x_), y(y_)
	{ }
	
	float x, y;
};

class Component {
};

class Entity {
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
	void print(Printer& printer) const {
		printer.print() << "Entity (" << _name << "):" << std::endl; 
	}
private:
	std::string _name;
	std::vector<Component*> _components;
	Vector2f _position;
};

class Scene {
public:
	virtual ~Scene() {
		for(std::size_t i = 0; i < _entities.size(); i++)
			delete _entities[i];
		_entities.clear();
	}
	void addEntity(Entity* entity) {
		_entities.push_back(entity);
	}
	void print(Printer& printer) const {
		printer.print() << "Scene:" << std::endl;
		printer.addIndent(1);
		for (std::size_t i = 0; i < _entities.size(); i++)
			_entities[i]->print(printer);
	}
private: 
	std::vector<Entity*> _entities;
};

class SceneBuilder {
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
protected:
	void assertEntity() {
		if (!_entity) {
			std::cout << "Builder error: No entity set" << std::endl;
			exit(1);
		}
	}
private:
	Scene& _scene;
	Entity* _entity;
};

class Texture {
public:
	void load(const std::string& path) {
	}
};

void print(const Scene& scene) {
	Printer printer;
	scene.print(printer);
	std::string what = printer.getStream().str();
	std::cout << what << std::endl;
}

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
	
	print(scene);
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	run();
	
    return 0;
}