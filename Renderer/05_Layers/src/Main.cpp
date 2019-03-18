#include "Window.h"
#include "DrawBatch.h"
#include "Entity.h"
#include "ShapeDrawer.h"
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

std::vector<sb::Entity*> entities;

void addToScene(sb::Entity& entity)
{
	entities.push_back(&entity);
}

bool compareComponents(const sb::Component* left, const sb::Component* right) {
	return left->getDrawDepth() < right->getDrawDepth();
}

inline void collectComponents(std::vector<sb::Component*>& components) {
	for (std::size_t i = 0; i < entities.size(); i++) {
		std::vector<sb::Component*> currentComponents = entities[i]->getComponents();
		components.insert(components.end(), currentComponents.begin(), currentComponents.end());
	}
}

inline void drawComponents(std::vector<sb::Component*>& components, sb::Window& window) {
	for (std::size_t i = 0; i < components.size(); i++) {
		sb::Entity* parent = components[i]->getParent();
		parent->drawComponent(components[i], window);
	}
}

void drawScene(sb::Window& window)
{
	std::vector<sb::Component*> components;

	collectComponents(components);

	std::stable_sort(components.begin(), components.end(), compareComponents);

	drawComponents(components, window);
}

void demo1() 
{
	sb::Window window;

	sb::Entity triangles;
	sb::Entity quad1;

	triangles.setPosition(-0.5f, 0.5f);
	quad1.setPosition(0, 0);

	sb::ShapeDrawer* triangleDrawer = new sb::ShapeDrawer(sb::ShapeDrawerStyle::Triangle);
	sb::ShapeDrawer* quadDrawer = new sb::ShapeDrawer(sb::ShapeDrawerStyle::Quad);
	quadDrawer->setDrawDepth(-1);

	quad1.addComponent(quadDrawer);
	triangles.addComponent(triangleDrawer);
	
	addToScene(triangles);
	addToScene(quad1);

	while (window.isOpen()) {
		window.update();
		
		window.clear();
		drawScene(window);

		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Application: Build %s %s", __DATE__, __TIME__);

	demo1();
}
