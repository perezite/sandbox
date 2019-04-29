#include <iostream>
#include <algorithm>

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

void demo0() {
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

void demo1() {
	MyEntity2 entity(0.5f, 0.5f);

	while (true) {
		entity.update();
		entity.print();
		std::cin.get();
	}
}

void main() {
	demo1();

	// demo0();
}