#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


class MyEntity {
	float _x, _y;

public:
	MyEntity(float x = 0, float y = 0) 
		: _x(x), _y(y) 
	{ }
	
	void update() {
		_x = std::max(_x - 0.1f, 0.0f);
		_y = std::max(_y - 0.1f, 0.0f);
	}

	void print() {
		std::cout << _x << " " << _y << std::endl;
	}
};

void demo1() {
	MyEntity entity(0.5f, 0.5f);

	while (true) {
		entity.update();
		entity.print();
		std::cin.get();
	}
}

class Entity {
	float _x, _y;

public:
	Entity(float x = 0, float y = 0)
		: _x(x), _y(y)
	{ }

	inline float getX() const { return _x; }

	inline float getY() const { return _y; }

	inline void setX(float x) { _x = x; }

	inline void setY(float y) { _y = y; }
};

class Physics {
	Entity& _entity;

public:
	Physics(Entity& entity)
		: _entity(entity)
	{ }

	void update() {
		_entity.setX(std::max(_entity.getX() - 0.1f, 0.0f));
		_entity.setY(std::max(_entity.getY() - 0.1f, 0.0f));
	}
};

class MyEntity2 : public Entity {
	Physics _physics;

public:
	MyEntity2(float x = 0, float y = 0)
		: Entity(x, y), _physics(*this)
	{ }

	void update() {
		_physics.update();
	}

	void print() {
		std::cout << getX() << " " << getY() << std::endl;
	}
};

void demo2() {
	MyEntity2 entity(0.5f, 0.5f);

	while (true) {
		entity.update();
		entity.print();
		std::cin.get();
	}
}

class Entity3;

class Component3 {
	Entity3* _entity;

public:
	virtual ~Component3() { }

	virtual void update() = 0;

	inline Entity3& getEntity() { return *_entity; }

	inline void setEntity(Entity3& entity) { _entity = &entity; }
};

class Entity3 {
	std::vector<Component3*> _components;

	float _x, _y;

public:
	Entity3(float x = 0, float y = 0)
		: _x(x), _y(y)
	{ }

	virtual ~Entity3() {
		for (std::size_t i = 0; i < _components.size(); i++)
			delete _components[i];
	}

	inline float getX() const { return _x; }

	inline float getY() const { return _y; }

	inline void setX(float x) { _x = x; }

	inline void setY(float y) { _y = y; }

	void addComponent(Component3* component) {
		component->setEntity(*this);
		_components.push_back(component);
	}

	void updateComponents() {
		for (std::size_t i = 0; i < _components.size(); i++)
			_components[i]->update();
	}
};

class Physics3 : public Component3 {
public:
	virtual void update() {
		getEntity().setX(std::max(getEntity().getX() - 0.1f, 0.0f));
		getEntity().setY(std::max(getEntity().getY() - 0.1f, 0.0f));
	}

	virtual ~Physics3() 
	{
		std::cout << "Physics::dtor()" << std::endl;
	}
};

class MyEntity3 : public Entity3 {
public:
	MyEntity3(float x, float y)
		: Entity3(x, y)
	{
		addComponent(new Physics3());
	}

	void update() {
		updateComponents();
	}

	void print() {
		std::cout << getX() << " " << getY() << std::endl;
	}
};

void demo3() {
	MyEntity3 entity(0.5f, 0.5f);

	while (true) {
		entity.update();
		entity.print();
		std::string input;
		std::getline(std::cin, input);
		if (input == "exit")
			break;
	}
}

void main() {
	demo3();

	// demo2();

	// demo1();
}

