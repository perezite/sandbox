#include "Window.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include "Triangle.h"
#include "Quad.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>

//class Illuminatus : sb::Drawable 
//{
//public:
//	Illuminatus() : sb::Drawable(IlluminatusMesh) { }
//
//private:
//	static const sb::Mesh IlluminatusMesh;
//};
//
//const sb::Mesh Illuminatus::IlluminatusMesh = sb::Mesh({
//	sb::Vertex(sb::Vector2f(-0.5f, -0.5f), sb::Color(1, 0, 0, 1)),
//	sb::Vertex(sb::Vector2f(0.5f, -0.5f), sb::Color(0, 1, 0, 1)),
//	sb::Vertex(sb::Vector2f(0.0f,  0.5f), sb::Color(0, 0, 1, 1))
//}, sb::PrimitiveType::Triangles);

class Illuminatus : sb::Drawable
{
	Illuminatus()
	{
		// m_mainTriangle
	}

private:
	sb::Triangle m_mainTriangle;

	sb::Triangle m_subTriangles[3];

};

void demo1() 
{
	sb::Window window;

	sb::Triangle triangle;
	triangle.setPosition(0.5f, 0.5f);
	triangle.setRotation(45 * sb::Transform::ToRadians);
	triangle.setScale(0.5f, 1.5f);

	sb::Quad quad;
	quad.setPosition(-0.25f, -0.5f);
	quad.setRotation(-10 * sb::Transform::ToRadians);
	quad.setScale(2, 0.25f);

	while (window.isOpen()) {
		window.update();
		window.clear();
		window.draw(triangle);
		window.draw(quad);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	demo1();
}
