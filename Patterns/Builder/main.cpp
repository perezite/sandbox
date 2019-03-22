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
private: 
	std::string _path;	
public:
	void load(const std::string& path) { 
		_path = path;
	}
	inline const std::string& getPath() { return _path; }
};

class Window;
class Drawable {
	virtual void draw(Window& window, const Vector2f& position) = 0;
};

class Window {
public:
	void draw(const std::vector<Vector2f>& vertices) {
		std::cout << "Window::draw(): ";
		for (std::size_t i = 0; i < vertices.size(); i++)
			std::cout << "(" << vertices[i].x << "," << vertices[i].y << ") ";
		std::cout << std::endl;
	}
};

class Sprite : public Drawable {
private:
	Texture& _texture;
public:
	Sprite(Texture& texture)
		: _texture(texture)
	{ }
	void draw(Window& window, const Vector2f& position) {
		std::cout << "Sprite::draw()" << std::endl;
		std::vector<Vector2f> vertices(4);
		vertices[0] = Vector2f(position.x - 0.5f, position.y - 0.5f);
		vertices[1] = Vector2f(position.x + 0.5f, position.y - 0.5f);
		vertices[2] = Vector2f(position.x + 0.5f, position.y + 0.5f);
		vertices[3] = Vector2f(position.x - 0.5f, position.y + 0.5f);				
		window.draw(vertices);
	}
};

class Scene;
class Component : public Drawable {
private:
	Vector2f _position;
	std::string _type;
public:
	Component(const std::string& type) 
		: _type(type)	
	{ }
	inline const Vector2f& getPosition() const { return _position; }	
	inline const std::string& getType() const { return _type; } 
	virtual void update(Scene& scene) { };
	virtual void draw(Window& window, const Vector2f& position) { };
	inline const Vector2f& getPosition() { return _position; }
};

class Paddle : public Component {
public:
	Paddle() 
		: Component("Paddle")
	{ }
	void update(Scene& scene) {
		std::cout << "Paddle::update()" << std::endl;
	}
};

class Propulsion : public Component {
private:
	bool _left;
public:
	Propulsion(bool left) 
		: Component("Propulsion"), _left(left)
	{ }
	void update() { 
		std::cout << "Propulsion::update()" << std::endl;
	}
	void draw(Window& window, const Vector2f& position) {
		std::cout << "Propulsion::draw()" << std::endl;
	}
};

class Drawer : public Component {
private:
	Drawable& _drawable;
public:
	Drawer(Drawable& drawable) 
		: Component("Drawer"), _drawable(drawable)
	{ }
	void update() { 
		std::cout << "Drawer::update()" << std::endl;
	}
	void draw(Window& window, const Vector2f& position) {
		std::cout << "Drawer::draw()" << std::endl;
	}
};

class Entity {
private:
	std::string _name;
	Entity* _parent;
	std::vector<Component*> _components;
	Vector2f _position;	
	Vector2f _scale;
public:
	Entity(const std::string& name, Entity* parent = NULL) 
		: _name(name), _parent(parent), _scale(1,1)
	{ }
	virtual ~Entity() {
		for(std::size_t i = 0; i < _components.size(); i++)
			delete _components[i];
		_components.clear();
	}
	inline void setPosition(const Vector2f& position) { _position = position; }
	inline void setScale(const Vector2f& scale) { _scale = scale; }
	inline const std::string& getName() const { return _name; }
	inline Entity* getParent() const { return _parent; }
	inline const Vector2f& getPosition() const { return _position; }
	inline const Vector2f& getScale() const { return _scale; }	
	inline const std::vector<Component*>& getComponents() const { return _components; }
	void addComponent(Component* component) {
		_components.push_back(component);
	}
	void update(Scene& scene) {
		std::cout << "Entity::update() (" << getName() << ")" << std::endl;
		for (std::size_t i = 0; i < _components.size(); i++) 
			_components[i]->update(scene);
	}
	void draw(Window& window, const Vector2f& position) {
		std::cout << "Entity::draw() (" << getName() << ")" << std::endl;
		for (std::size_t i = 0; i < _components.size(); i++)
			_components[i]->draw(window, position);
	}
};

class Scene : public Drawable {
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
	void draw(Window& window, const Vector2f& position) {
		for (std::size_t i = 0; i < _entities.size(); i++)
			_entities[i]->draw(window, position);
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
	SceneBuilder& addEntity(const std::string& name="unnamed") {
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
	SceneBuilder& withScale(const Vector2f& scale) {
		assertEntity();
		_entity->setScale(scale);
		return *this;
	}	
	SceneBuilder& withPosition(float x, float y) {
		return withPosition(Vector2f(x, y));
	}
	SceneBuilder& withScale(float x, float y) {
		return withScale(Vector2f(x, y));
	}	
	SceneBuilder& withChildEntity(const std::string name="unnamed") {
		_entity = new Entity(name, _entity);
		_scene.addEntity(_entity);
		return *this;	
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
		const Vector2f position = component.getPosition();
		std::cout << indent(3) << "Position (" << position.x << "," << 
			position.y << ")" << std::endl; 
	}
	void print(const Entity& entity) {
		auto position = entity.getPosition();
		auto scale = entity.getScale();
		std::cout << indent(1) << "Entity (" << entity.getName() << "):" << std::endl;
		std::cout << indent(2) << "Position: (" << position.x << 
			"," << position.y << ")" << std::endl; 
		std::cout << indent(2) << "Scale: (" << scale.x << 
			"," << scale.y << ")" << std::endl; 			
		if (entity.getParent())
			std::cout << indent(2) << "Parent: " << entity.getParent()->getName() << std::endl;
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
	Window window;
	Scene scene;
	
	Texture paddleTexture;
	Texture propulsionTexture;
	paddleTexture.load("paddle.png");
	propulsionTexture.load("propulsion.png");
	Sprite paddleSprite(paddleTexture);
	// ParticleSystem particleSystem(propulsionTexture);
	
	SceneBuilder(scene).addEntity("paddle")
		.withPosition(1, 1)
		.withComponent(new Paddle())
			// withScale(0.5f, 0.5f)
		.withComponent(new Drawer(paddleSprite))
		.withChildEntity("leftPropulsion")
			.withPosition(-0.5f, -0.2f)
			.withScale(0.1f, 0.1f)
			.withComponent(new Propulsion(false));
			// .withComponent(new Drawer(particleSystem))
				// .withNumParticles(200)
			// .withComponent(new PropulsionExtra());

	ScenePrinter().print(scene);		
	scene.update();
	scene.draw(window, Vector2f(0,0));
}

int main() {
	std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
	run();
	
    return 0;
}