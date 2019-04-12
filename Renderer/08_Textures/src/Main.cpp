#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Triangle.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <string>

void demo1() {
	sb::Window window;
	sb::Quad quad;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear();
		window.draw(quad);
		window.display();
	}
}

class MyTexture {

};

class MyTransform {

};

struct MyRenderStates {
	MyTransform transform;

	const MyTexture* texture;

	MyRenderStates()
	{ }

	MyRenderStates(const MyTransform& transform_, const MyTexture& texture_)
		: transform(transform_), texture(&texture_)
	{ }

	MyRenderStates(const MyTransform& transform_)
		: transform(transform_), texture(NULL)
	{ }

	MyRenderStates(const MyTexture& texture_)
		: transform(MyTransform()), texture(&texture_)
	{ }
};

void argumentTest(MyRenderStates states) {
	
}

void demo2() {
	MyTransform transform;
	MyTexture texture;

	MyRenderStates states;
	states.texture = &texture;
	states.transform = transform;

	argumentTest(transform);
	argumentTest(texture);
	argumentTest(states);
}

void demo3() {
	sb::Window window;
	sb::Shader shader;

	const std::string vertexShader =
		"attribute vec2 position;																\n"
		"attribute vec4 color;																	\n"
		"uniform mat3 transform;																\n"
		"varying vec4 v_color;																	\n"
		"vec3 transformedPosition;																\n"
		"void main()																			\n"
		"{																						\n"
		"	transformedPosition = transform * vec3(position.x, position.y, 1);					\n"
		"	gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0, 1);				\n"
		"	v_color = color;																	\n"
		"}																						\n";

	const std::string fragmentShader =
		"#version 100									\n"
		"precision mediump float;						\n"
		"varying vec4 v_color;		 					\n"
		"void main()									\n"
		"{ \n											  "
		"	gl_FragColor = v_color;						\n"
		"}												\n";

	shader.loadFromMemory(vertexShader, fragmentShader);
}

void demo4() {
	sb::Window window;
	sb::Shader shader;
	shader.loadFromAsset("Shaders/diffuse.vs", "Shaders/diffuse.fs");
}

void demo5() {
	sb::Window window;
	
	sb::DrawBatch batch;
	sb::Quad quad;
	std::vector<sb::Triangle> triangles(2);
	triangles[0].setScale(0.5f, 0.5f);
	triangles[0].setPosition(-0.5f, -0.5f);
	triangles[1].setScale(0.5f, 0.5f);
	triangles[1].setPosition(0.5f, 0.5f);
	quad.setScale(0.25f, 0.25f);
	quad.setPosition(-0.5f, 0.5f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear();
		batch.draw(triangles[0]);
		batch.draw(triangles[1]);
		window.draw(batch);
		window.draw(quad);
		window.display();
	}
}

void demo7() {
	sb::Window window;
	
	sb::Quad quad;
	/*quad.getMesh()[0].color = sb::Color(1, 0, 0, 1);
	quad.getMesh()[1].color = sb::Color(0, 1, 0, 1);
	quad.getMesh()[2].color = sb::Color(0, 0, 1, 1);
	quad.getMesh()[3].color = sb::Color(0, 1, 1, 1);*/

	sb::Texture texture("Textures/GreenBlock.png");
	sb::Shader shader("Shaders/Diffuse.vert", "Shaders/Diffuse.frag");
	sb::DrawStates states;
	states.texture = &texture;
	states.shader = &shader;
	
	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear(sb::Color(1, 1, 1, 1));
		// window.draw(quad);
		window.draw(quad, states);
		window.display();
	}
}

void disco(sb::Quad& quad) {
	sb::Mesh& mesh = quad.getMesh();
	mesh[0].color = sb::Color(1, 0, 0, 1);
	mesh[1].color = sb::Color(0, 1, 0, 1);
	mesh[2].color = sb::Color(0, 0, 1, 1);
	mesh[3].color = sb::Color(0, 1, 1, 1);
}

void demo6() {
	sb::Window window;

	sb::Quad quad;
	disco(quad);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear();
		window.draw(quad);
		window.display();
	}
	
}

int main(int argc, char* args[])
{
	SDL_Log("Texture Renderer: Build %s %s", __DATE__, __TIME__);

	demo7();

	// demo6();

	//demo5();

	//demo4();

	//demo3();

	//demo2();

	//demo1();
}
