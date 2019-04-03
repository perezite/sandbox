#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class Window;
class Drawable {
public:
	virtual void draw(const Window& window) = 0;
};

class Window {
public:
	void update() { }
	void draw(Drawable& drawable) const {
		drawable.draw(*this);
	}
	void display() const {
		std::cout << "Window::display(), Press any key for next frame" << std::endl;
		std::cin.get();
	}
};

class Component : public Drawable {
	std::string _name;
public:
	Component(std::string name) : _name(name)
	{ }
	virtual void update() {
		std::cout << "Component::update(" << _name << ")" << std::endl;
	}
	virtual void draw(const Window& window) {
		std::cout << "Component::draw(" << _name << ")" << std::endl;
	}
};

class Entity : public Drawable {
	std::string _name;
	std::vector<Component*> _components;
public:
	Entity()
	{ }
	virtual ~Entity() {
		for (std::size_t i = 0; i < _components.size(); i++)
			delete _components[i];
		_components.clear();
	}
	virtual void update() {
		for (std::size_t i = 0; i < _components.size(); i++)
			_components[i]->update();
	}
	virtual void draw(const Window& window) {
		for (std::size_t i = 0; i < _components.size(); i++)
			_components[i]->draw(window);
	}
	void addComponent(Component* component) { 
		_components.push_back(component); 
	}
};

struct Scene : public Drawable {
	virtual ~Scene() {
		for (std::size_t i = 0; i < _entities.size(); i++)
			delete _entities[i];
		_entities.clear();
	}
	virtual void initialize() { }
	void update() { 
		for (std::size_t i = 0; i < _entities.size(); i++)
			_entities[i]->update();
	}
	void draw(const Window& window) {
		for (std::size_t i = 0; i < _entities.size(); i++)
			_entities[i]->draw(window);
	}
	Entity& addEntity(Entity* entity) {
		_entities.push_back(entity);
		return *entity;
	}
	virtual bool isOpen() const { return _isOpen; }
	bool close() { _isOpen = false; }
private:
	bool _isOpen;
	std::vector<Entity*> _entities;
};

void run(Scene& scene, Window& window) {
	while (scene.isOpen()) {
		window.update();
		scene.update();
		window.draw(scene);
		window.display();
	}
}

void scene() {
	Window window;
	Scene scene;

	Entity& firstEntity = scene.addEntity(new Entity());
	Entity& secondEntity = scene.addEntity(new Entity());
	firstEntity.addComponent(new Component("TestComponent"));
	secondEntity.addComponent(new Component("TestComponent2"));
	secondEntity.addComponent(new Component("TestComponent3"));

	run(scene, window);
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	scene();
	std::cin.get();

	return 0;
}
