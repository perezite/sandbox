#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class Window2;
class Drawable2 {
public:
	virtual void draw(const Window2& window) = 0;
};

class Window2 {
public:
	void update() { }
	void draw(Drawable2& drawable) const {
		drawable.draw(*this);
	}
	void display() const {
		std::cout << "Window2::display(), Press any key for next frame" << std::endl;
		std::cin.get();
	}
};

class Component : public Drawable2 {
	std::string _name;
public:
	Component(std::string name) : _name(name)
	{ }
	virtual void update() {
		std::cout << "Component::update(" << _name << ")" << std::endl;
	}
	virtual void draw(const Window2& window) {
		std::cout << "Component::draw(" << _name << ")" << std::endl;
	}
};

class Entity : public Drawable2 {
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
	virtual void draw(const Window2& window) {
		for (std::size_t i = 0; i < _components.size(); i++)
			_components[i]->draw(window);
	}
	void addComponent(Component* component) { 
		_components.push_back(component); 
	}
};

struct Scene : public Drawable2 {
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
	void draw(const Window2& window) {
		for (std::size_t i = 0; i < _entities.size(); i++)
			_entities[i]->draw(window);
	}
	Entity& addEntity(Entity* entity) {
		_entities.push_back(entity);
		return *entity;
	}
	virtual bool isOpen() const { return _isOpen; }
	void close() { _isOpen = false; }
private:
	bool _isOpen;
	std::vector<Entity*> _entities;
};

void runScene1(Scene& scene, Window2& window) {
	std::size_t counter = 0;
	while (scene.isOpen()) {
		window.update();
		scene.update();
		window.draw(scene);
		window.display();

		counter++;
		if (counter >= 3)
			scene.close();
	}
}

void scene1(Window2& window) {
	Scene scene;

	Entity& firstEntity = scene.addEntity(new Entity());
	Entity& secondEntity = scene.addEntity(new Entity());
	firstEntity.addComponent(new Component("TestComponent"));
	secondEntity.addComponent(new Component("TestComponent2"));
	secondEntity.addComponent(new Component("TestComponent3"));

	runScene1(scene, window);
}

void runScene2(Scene& scene, Window2& window) {
	while (scene.isOpen()) {
		window.update();
		scene.update();
		window.draw(scene);
		window.display();
		scene.close();
	}
}

void scene2(Window2& window) {
	Scene scene;

	Entity& firstEntity = scene.addEntity(new Entity());
	firstEntity.addComponent(new Component("TestComponent4"));
	runScene2(scene, window);

	std::cout << "The End!" << std::endl;
	std::cin.get();
}

void demo0() {
	Window2 window;
	scene1(window);
	scene2(window);
}

struct Nameable {
	std::string name;
};

class Window;

class Drawable : public Nameable {
public:
	virtual void draw(Window& window) = 0;
};

class Window : public Nameable {
public:
	bool isOpen() { return true; }
	void update() { }
	void display() { }
	void draw(Drawable& drawable) {
		std::cout << "Window=" << name << "::draw(Drawable=" << drawable.name << ") " << std::endl;
	}
};

class Texture : public Nameable {
	std::string _assetPath;
public:
	void loadFromAsset(const std::string& assetPath) {
		_assetPath = assetPath;
	}
};

template <class T>
struct Vector2 {
	T x;
	T y;
	Vector2(T x_, T y_) : x(x_), y(y_)
	{ }
	Vector2(T v_) : x(v_), y(v_)
	{ }
};

typedef Vector2<float> Vector2f;

class Transformable {
	Vector2f _position;
	Vector2f _scale;
public:
	inline void setPosition(const Vector2f& position) { _position = position; }
	inline void setScale(const Vector2f& scale) { _scale = scale; }
};

class Sprite : public Drawable {
	Texture* _texture;
public:
	void setTexture(Texture& texture) {
		_texture = &texture;
	}

	virtual void draw(Window& window) {
		window.draw(*this);
	}
};

void demo98() {
	Window window;
	Texture blockTex;
	Sprite block;

	window.name = "myWindow";
	block.name = "myBlock";

	while(window.isOpen()) {
		block.draw(window);
		std::cin.get();
	}

	//DrawBatch batch(512);
	//Window window;
	//Texture blockTex;
	//Sprite block;

	/*blockTex.loadFromAsset("block.png");
	block.setTexture(blockTex);
	block.setScale(0.5f);
	block.getConfetti().setPosition(-0-5f);
	block.getConfetti().setScale(0.2f, 0.2f);

	while(window.isOpen()) {
		window.update();
		block.update();
		block.getConfetti().update();

		block.draw(batch);
		batch.draw(window);
		block.getConfetti().draw(block.getTransform());
		window.display();
	}

	*/
}

void demo99() {
	/*
	DrawBatch batch(512);
	Window window;
	Texture blockTex;
	Scene scene;

	blockTex.loadFromAsset("block.png");
	auto block = scene.add<Sprite>();
	block.setTexture(blockTex);
	block.setScale(0.5f);
	auto confetti = block.add<Confetti>(50); // number of confetti particles
	confetti.setPosition(-0.5f);
	confetti.setScale(0.2f);
	confetti.setDrawLayer(1);		// default is 0

	while(window.isOpen()) {
		scene.update();
		scene.draw(batch);
		batch.draw(window);
		window.display();
	}
	*/
}

void demo() {
	demo98();
	
	//demo0();
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	demo();

	return 0;
}


