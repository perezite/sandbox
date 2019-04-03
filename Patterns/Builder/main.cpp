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
public:
    virtual void draw(Window& window, const Vector2f& position) = 0;
};

class Transformable {
private: 
    Vector2f _position;	
    Vector2f _scale;
public:
    Transformable() : _position(0, 0), _scale(1, 1) { }
    inline const Vector2f& getPosition() const { return _position; }
    inline const Vector2f& getScale() const { return _scale; }	
    inline void setPosition(const Vector2f& position) { _position = position; }
    inline void setScale(const Vector2f& scale) { _scale = scale; }	
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
class Component : public Drawable, public Transformable {
private:
    std::string _type;
public:
    Component(const std::string& type) 
        : _type(type)	
    { }
	virtual ~Component() {
		std::cout << "~Component()" << std::endl; 
	}
    inline const std::string& getType() const { return _type; } 
    virtual void update(Scene& scene) { };
    virtual void draw(Window& window, const Vector2f& position) { };
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
        std::cout << "Propulsion::draw() left=" << _left << std::endl;
    }
};

class ParticleSystem : public Drawable {
private:
    std::size_t _numParticles;
public:
    ParticleSystem() 
        : _numParticles(200)
    { }
    inline void setNumParticles(std::size_t numParticles) { _numParticles = numParticles; }
    void draw(Window& window, const Vector2f& position) {
        std::cout << "ParticleSystem::draw() with " << _numParticles << " particles" << std::endl;
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
        _drawable.draw(window, position);
    }
};

class Entity : public Transformable {
private:
    std::string _name;
    Entity* _parent;
    std::vector<Component*> _components;
public:
    Entity(const std::string& name, Entity* parent = NULL) 
        : _name(name), _parent(parent)
    { }
    virtual ~Entity() {
        for(std::size_t i = 0; i < _components.size(); i++)
            delete _components[i];
        _components.clear();
    }
    inline const std::string& getName() const { return _name; }
    inline Entity* getParent() const { return _parent; }
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
    Transformable* _current;
protected:
    Entity* getEntity() {
        if (!_entity) {
            std::cout << "Builder error: No entity set" << std::endl;
            exit(1);
        }
        return _entity;; 
    }
    Transformable* getCurrent() {
        if (!_current) {
            std::cout << "Builder error: No transformable set" << std::endl;
            exit(1);
        }
        return _current;			
    }	
	SceneBuilder& addComponent(Component* component) {
		getEntity()->addComponent(component);
		_current = component;
		return *this;
	}
public:
    SceneBuilder(Scene& scene) 
        : _scene(scene), _entity(NULL), _current(NULL)
    { }
    SceneBuilder& addEntity(const std::string& name="unnamed") {
        _entity = new Entity(name);
        _current = _entity;
        _scene.addEntity(_entity);		
        return *this;
    }
    template <class C>
    SceneBuilder& withComponent() {
		return addComponent(new C());
    }
	template <class C, class Arg1>
	SceneBuilder& withComponent(const Arg1& arg1) {
		return addComponent(new C(arg1));
	}
	template <class C, class Arg1>
	SceneBuilder& withComponent(Arg1& arg1) {
		return addComponent(new C(arg1));
	}
    SceneBuilder& withComponent(Drawable& drawable) {
        return withComponent<Drawer>(drawable);
    }
    SceneBuilder& withPosition(const Vector2f& position) {
        getCurrent()->setPosition(position);
        return *this;
    }
    SceneBuilder& withScale(const Vector2f& scale) {
        getCurrent()->setScale(scale);
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
        _current = _entity;
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
    void printTransformable(const Transformable& transformable, std::size_t indent_) {
        const Vector2f& position = transformable.getPosition();
        const Vector2f& scale = transformable.getScale();
        std::cout << indent(indent_) << "Position: (" << position.x << 
            "," << position.y << ")" << std::endl; 
        std::cout << indent(indent_) << "Scale: (" << scale.x << 
            "," << scale.y << ")" << std::endl; 	
    }
    void print(const Component& component) {
        std::cout << indent(2) << "Component (" << component.getType() << ")" << std::endl; 
        printTransformable(component, 3);
    }
    void print(const Entity& entity) {
        auto position = entity.getPosition();
        auto scale = entity.getScale();
        std::cout << indent(1) << "Entity (" << entity.getName() << "):" << std::endl;
        printTransformable(entity, 2);
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
	ParticleSystem particleSystem;
    particleSystem.setNumParticles(42);
    
    SceneBuilder(scene).addEntity("paddle")
        .withPosition(1, 1)
        .withComponent<Paddle>()
            .withScale(0.5f, 0.5f)
        .withComponent(paddleSprite)
        .withChildEntity("leftPropulsion")
            .withPosition(-0.5f, -0.2f)
            .withScale(0.1f, 0.1f)
            .withComponent<Propulsion>(false)
            .withComponent(particleSystem);

    ScenePrinter().print(scene);		
    scene.update();
    scene.draw(window, Vector2f(0,0));
}

int main() {
    std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;
    run();
	std::cin.get();
    
    return 0;
}