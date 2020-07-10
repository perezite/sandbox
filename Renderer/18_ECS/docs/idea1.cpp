#include <Orbis.h>

void updateRocket(Sprite& rocket, Updating updating) {
	if (Keyboard::isKeyDown(Key::Left)) 
		rocket.move(-updating.deltaSeconds * 3, 0);
}

template <class TEntity>
class Behaviour : Component<TEntity> {
	...
}

template <class TEntity>
class BaseEntity {
	template <
		template <typename> class TComponent
	>
	void createComponent() {
		TComponent<TEntity> component();
		components.add(component);
	}

	...
}

void main() {
	Window window;
	Scene scene;
	
	auto& rocket = scene.create<Sprite>("rocket.png");
	rocket.createComponent<Behaviour>().setMethod(updateRocket);
	rocket.createChild<ParticleSystem>().setPosition(0, -.5f);
	
	while (window.isOpen()) {
		window.update();
		scene.update();
		
		scene.draw(window);
		window.display();
	}
}