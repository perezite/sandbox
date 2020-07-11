#include <iostream>
#include <vector>
#include <deque>
using namespace std;

class BaseEntity {  
    float _position;
public:
    void setPosition(float x) { _position = x; }
    
    float getPosition() const { return _position; }
};

class Component {  
protected:
    BaseEntity* _entity;
public:  
    virtual ~Component() { }
    void setEntity(BaseEntity& entity) {
        _entity = &entity;
    }
        
    virtual void update(BaseEntity& entity) = 0;
};

template <class TEntity>
class Entity : public BaseEntity {
    vector<Component*> _components;
public:
    virtual ~Entity() {
        for (size_t i = 0; i < _components.size(); i++)
            delete _components[i];
    }

    vector<Component*>& getComponents() { return _components; }

    template< template <typename> class TComponent >
    void createComponent() {
        TEntity* instance = (TEntity*)(this);
        TComponent<TEntity>* component = new TComponent<TEntity>();
        component->setEntity(*instance);
        _components.push_back(component);
    }
    
    template<class TComponent>
    void createComponent() {
        TComponent* component = new TComponent();
        _components.push_back(component);
    }
};

class Sprite : public Entity<Sprite> {
public:
    void doSomething() {
        cout << "Sprite::doSomething()" << endl;
    }
    
    void doSomethingSpritey() {
        cout << "Sprite::doSomethingSpritey()" << endl;
    }
};

template <class TEntity>
class Behaviour : public Component {
public:    
    virtual void update(BaseEntity& baseEntity) {
        auto& entity = (TEntity&)baseEntity;
        cout << "Behaviour::update()" << endl;
        entity.doSomethingSpritey();
    }
};

class SimpleComponent : public Component {
public:
    virtual void update(BaseEntity& entity) {
        cout << "SimpleComponent::update()" << endl;
        cout << "Old position: " << entity.getPosition() << endl;
        entity.setPosition(3);
        cout << "New position: " << entity.getPosition() << endl;
    }
};

int main() {    
    Sprite sprite;
    sprite.createComponent<Behaviour>();
    sprite.createComponent<SimpleComponent>();
 
    auto& components = sprite.getComponents();
    for (size_t i = 0; i < components.size(); i++)
        components[i]->update(sprite);
    
    return 0;
}