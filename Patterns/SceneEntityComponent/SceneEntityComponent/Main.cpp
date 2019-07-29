#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>

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

struct Scene2 : public Drawable2 {
	virtual ~Scene2() {
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

void runScene1(Scene2& scene, Window2& window) {
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
	Scene2 scene;

	Entity& firstEntity = scene.addEntity(new Entity());
	Entity& secondEntity = scene.addEntity(new Entity());
	firstEntity.addComponent(new Component("TestComponent"));
	secondEntity.addComponent(new Component("TestComponent2"));
	secondEntity.addComponent(new Component("TestComponent3"));

	runScene1(scene, window);
}

void runScene2(Scene2& scene, Window2& window) {
	while (scene.isOpen()) {
		window.update();
		scene.update();
		window.draw(scene);
		window.display();
		scene.close();
	}
}

void scene2(Window2& window) {
	Scene2 scene;

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

class Dumpable {
public:
	virtual void dump() const { std::cout << "Generic dumpable"; };
};

template <class T>
struct Vector2 {
	T x;
	T y;
	Vector2(T x_ = 0, T y_ = 0) : x(x_), y(y_)
	{ }
	Vector2(T v_) : x(v_), y(v_)
	{ }
};
typedef Vector2<float> Vector2f;

inline Vector2f& operator+=(Vector2f& left, const Vector2f& right) {
	left.x += right.x;
	left.y += right.y;
	return left;
}

struct Transform : public Dumpable {
	Vector2f position;
	void dump() const {
		std::cout << "position: " << position.x << " " << position.y;
	}
};

Transform& operator *=(Transform& left, const Transform& right) {
	left.position += right.position;
	return left;
}

class Texture : public Dumpable {
	std::string _assetPath;
public:
	inline std::string assetPath() { return _assetPath; }
	virtual void dump() const {
		std::cout << "_assetPath=" << _assetPath << std::endl;
	}
	void loadFromAsset(const std::string& assetPath) {
		_assetPath = assetPath;
	}
};

struct DrawState {
	Transform transform;
	const Texture* texture = NULL;
	static DrawState default() {
		static DrawState state;
		return state;
	}
	DrawState() { }
	DrawState(const Transform& transform_) : transform(transform_)
	{ }
	DrawState(const Texture& texture_) : texture(&texture_)
	{ }
};

class DrawTarget;
class Drawable : public Nameable, public Dumpable {
public:
	virtual void draw(DrawTarget& window, DrawState& state = DrawState::default()) = 0;
};

class DrawTarget {
public:
	virtual void draw(DrawState& state) = 0;
};

class Window : public DrawTarget {
public:
	bool isOpen() { return true; }
	void update() { }
	void display() { }
	virtual void draw(DrawState& state) {
		std::cout << "Window::draw():" << std::endl;
		std::cout << "\tstate.transform=" << "\n\t\t";
		state.transform.dump();
		std::cout << std::endl;
		std::cout << "\tstate.texture=" << "\n\t\t";
		if (state.texture != NULL)
			state.texture->dump();
		else
			std::cout << "NULL";
		std::cout << std::endl;
	}
};

class Transformable {
	Transform _transform;
public:
	inline Transform& getTransform() { return _transform; }
	inline const Vector2f& getPosition() { return _transform.position; }
	inline void setPosition(const Vector2f& position) { _transform.position = position; }
};

class Sprite : public Drawable, public Transformable {
	Texture* _texture;
public:
	void setTexture(Texture& texture) {
		_texture = &texture;
	}
	virtual void draw(DrawTarget& target, DrawState& state) {
		state.texture = _texture;
		state.transform *= getTransform();
		target.draw(state);
	}
};

class DrawBatch : public DrawTarget, public Drawable {
	std::vector<Drawable*> _buffer;
public:
	DrawBatch() { }
	virtual void draw(DrawState& state) { }
	virtual void draw(DrawTarget& target, DrawState& state) { }
	virtual void dump() { }
};

class Confetti : public Drawable, public Transformable {
	size_t _count;
public:
	Confetti(size_t count) : _count(count)
	{ }
	virtual void draw(DrawTarget& target, DrawState& state) {
		std::cout << "[Confetti=" << name << "]::draw()" << std::endl;
		state.transform *= getTransform();
		target.draw(state);
	}
	void update() {
		std::cout << "[Confetti=" << name << "]::update()" << std::endl;
	}
};

class Block : public Sprite {
	Confetti _confetti;
public:
	Block() : _confetti(42) 
	{
		_confetti.name = "myConfetti";
	}
	void update() {
		std::cout << "[Block=" << name << "]::update()" << std::endl;
		_confetti.update();
	}
	inline Confetti& getConfetti() { return _confetti; }
	virtual void draw(DrawTarget& target, DrawState& state = DrawState::default()) {
		std::cout << "[Block=" << name << "]::draw()" << std::endl;
		Sprite::draw(target, state);
	}
	void drawConfetti(DrawTarget& target, DrawState& state = DrawState::default()) {
		state.transform *= getTransform();
		_confetti.draw(target, state);
	}
};

void demo0250() {
	DrawBatch batch;
	Window window;
	Texture blockTex;
	Block block;

	block.name = "myBlock";
	blockTex.loadFromAsset("myBlock.png");
	block.setTexture(blockTex);
	block.setPosition(Vector2f(1, 1));
	block.getConfetti().setPosition(Vector2f(0.5f, 0.5f));

	while(window.isOpen()) {
		window.update();
		block.update();
		block.draw(window);
		block.drawConfetti(window);
		std::cin.get();
		window.display();
	}
}

class BaseNamedType {
public:
	virtual const int getTypeId() const = 0;
};

const int generateTypeId() {
	static int typeId = -1;
	typeId++;
	return typeId;
}

template <class T>
class NamedType : public BaseNamedType {
public:
	static int getStaticTypeId() {
		static int typeId = generateTypeId();
		return typeId;
	}

	virtual const int getTypeId() const {
		return getStaticTypeId();
	}
};

class MySprite : public NamedType<MySprite> {
public:
	void update() {
		std::cout << "MySprite::update()" << std::endl;
	}
};

class MyParticleSystem : public NamedType<MyParticleSystem> {
public:
	void update() {
		std::cout << "MyParticleSystem::update()" << std::endl;
	}
};

template <class T>
T* find(std::vector<BaseNamedType*> namedTypes) {
	for (size_t i = 0; i < namedTypes.size(); i++) {
		if (namedTypes[i]->getTypeId() == T::getStaticTypeId())
			return (T*)namedTypes[i];
	}

	return NULL;
}

void demo0375() {
	Window window;
	Texture blockTex;

	std::vector<BaseNamedType*> namedTypes;
	namedTypes.push_back(new MySprite());
	namedTypes.push_back(new MyParticleSystem());

	std::cout << MySprite::getStaticTypeId() << std::endl;
	std::cout << MyParticleSystem::getStaticTypeId() << std::endl;
	std::cout << namedTypes[0]->getTypeId() << std::endl;
	std::cout << namedTypes[1]->getTypeId() << std::endl;

	auto particleSystem = find<MyParticleSystem>(namedTypes);
	particleSystem->update();
	auto sprite = find<MySprite>(namedTypes);
	sprite->update();

	for (size_t i = 0; i < namedTypes.size(); i++)
		delete namedTypes[i];

	std::cin.get();
}

class Empty { };

template <class T>
class Node {
	T *_element;
	std::vector<Node<T>*> _children;
public:
	Node() {
		_element = new T();
	}
	virtual ~Node() {
		for (size_t i = 0; i < _children.size(); i++)
			delete _children[i];
		delete _element;
	}
	inline std::vector<Node<T>*>& getChildren() { return _children; }
};

class Scene : public Node<Empty> {
public:
	template <class T>
	Node& add() {
				
		//getChildren()
	}
};

/*
class OtherBlockBehaviour {
	void update(Transformable& parent, float ds) {
		parent.rotate(3 * ds);
	}
}

class OtherBlock : public Sprite {
	OtherBlockBehaviour _behaviour;
	ParticleSystem _confetti;
public:
	OtherBlock {
		_confetti.setPosition(-0.5f);
		_confetti.setScale(0.2f);
	}

	void update(float ds) {
		_behaviour.update(*this, ds); 
	}

	virtual void draw(DrawTarget& target, DrawState state = DrawState::default()) {
		Sprite::draw(target, state);
		_confetti.draw(target, state);
	}
}
*/

void demo0500() {
	Window window;
	Texture blockTex;
	/* OtherBlock block;
	Stopwatch sw;

	blockTex.loadFromAsset("block.png");
	block.setTexture(blockTex);
	block.setScale(0.5f);

	sw.reset();

	while (window.isOpen()) {
		float ds = sw.reset();

		window.update();
		block.update(ds);

		window.clear();
		block.draw(window);
		block.getConfett().draw(window);
		window.display();
	}
	*/
}

/*
class BlockBehaviour : public Node {
public:
	virtual void update(Scene& scene) {
		scene.getParent(this).rotate(3 * scene.getDeltaSeconds());
	}
}

class Block : public Sprite {
public:
	Block() {
		auto confetti = addNode<ParticleSystem>();
		confetti.setPosition(-0.5f);
		confetti.setScale(0.2f);
		auto behaviour = addNode<BlockBehaviour>();
	}
}
*/

void demo1000() {
	Window window;
	Texture blockTex;
	Texture groundTex;
	Scene scene;

	/*blockTex.loadFromAsset("block.png");
	groundTex.loadFromAsset("ground.png");

	auto block = scene.addNode<Sprite>();
	block.setTexture(blockTex);
	block.setScale(0.5f);
	block.getNode<ParticleSystem>().setDrawLayer(1);	// default is 0

	auto ground = scene.addNode<Sprite>();
	ground.setTexture(groundTex);
	ground.setPosition(0, -0.4f);

	while(window.isOpen()) {
		window.update();
		scene.update();
		
		window.clear();
		scene.draw(window);
		window.display();
	}
	*/
}

void demo() {
	//demo0250();
	
	demo0375();

	//demo0();
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	demo();

	return 0;
}
