#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Triangle.h"
#include "Sprite.h"
#include "Math.h"
#include "Stopwatch.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

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

void demo6() {
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

void demo7() {
	sb::Window window;
	sb::Quad quad;
	sb::Mesh& mesh = quad.getMesh();
	for (std::size_t i = 0; i < 4; i++)
		mesh[i].color = sb::Color(1, 1, 1, 1);

	sb::Texture texture("Textures/GreenBlock.png");
	sb::Shader shader("Shaders/Diffuse.vert", "Shaders/Diffuse.frag");
	sb::DrawStates states;
	states.texture = &texture;
	states.shader = &shader;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad, states);
		window.display();
	}
}

void demo8() {
	sb::Window window;
	sb::Quad quad;
	sb::Mesh& mesh = quad.getMesh();
	for (std::size_t i = 0; i < 4; i++)
		mesh[i].color = sb::Color(1, 1, 1, 1);
	sb::Texture texture("Textures/GreenBlock.png");

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad, texture);
		window.display();
	}
}

void demo9() {
	sb::Window window;
	sb::Texture texture("Textures/GreenBlock.png");
	sb::Sprite sprite;
	sprite.setTexture(&texture);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear(sb::Color(0, 0, 0, 1));
		
		static int i = 0; 
		if (i % 2 == 0)
			sprite.setTexture(NULL);
		else
			sprite.setTexture(&texture);
		i++;

		window.draw(sprite);
		window.display();
	}
}

class Paramecium {
	const float epsilon = 0.01f;
	const float scaleSpeed = 0.1f;
	const sb::Vector2f speedRange = sb::Vector2f(0.5f, 0.7f);
	const sb::Vector2f positionRange = sb::Vector2f(-1, 1);
	const sb::Vector2f scaleRange = sb::Vector2f(0, 0.3f);
	const sb::Vector2f rotationRange = sb::Vector2f(0, 2 * sb::Pi);

	float speed;
	sb::Stopwatch sw;
	sb::Sprite sprite;
	sb::Vector2f targetPosition;
	float targetScale;

protected:
	void move(float ds) {
		sb::Vector2f distance = (targetPosition - sprite.getPosition());
		sb::Vector2f direction = distance.normalized();
		sprite.setPosition(sprite.getPosition() + ds * speed * direction);

		if (distance.getLength() < epsilon) {
			targetPosition = sb::Vector2f(sb::random2D(positionRange.x, positionRange.y));
			speed = sb::random(speedRange.x, speedRange.y);
		}
	}

	void wobble(float ds) {
		float currentScale = sprite.getScale().x;
		float direction = targetScale > currentScale ? 1.0f : -1.0f;
		float newScale = currentScale + ds * direction * scaleSpeed;
		sprite.setScale(newScale, newScale);

		if (fabs(targetScale - currentScale) < epsilon) {
			targetScale = sb::random(scaleRange.x, scaleRange.y);
		}
	}

public:
	Paramecium()
		: targetPosition(sb::random2D(positionRange.x, positionRange.y)), 
		targetScale(sb::random(scaleRange.x, scaleRange.y)),
		speed(sb::random(speedRange.x, speedRange.y))
	{
		targetPosition = sb::Vector2f(0, 0);

		sb::Vector2f position(sb::random2D(positionRange.x, positionRange.y));
		sprite.setPosition(position);

		sb::Vector2f scale(sb::random(scaleRange.x, scaleRange.y));
		sprite.setScale(scale);

		sprite.setRotation(sb::random(rotationRange.x, rotationRange.y));
	}

	inline void setTexture(sb::Texture* texture) { sprite.setTexture(texture); }

	void update(float ds) {
		move(ds);
		wobble(ds);
	}

	void draw(sb::DrawTarget& target) {
		target.draw(sprite);
	}
};

struct Scene10 {
	std::vector<sb::Texture> textures;
	std::vector<Paramecium> paramecia;

	void initTextures() {
		textures[0].loadFromAsset("Textures/CyanBlock.png");
		textures[1].loadFromAsset("Textures/GreenBlock.png");
		textures[2].loadFromAsset("Textures/PurpleBlock.png");
		textures[3].loadFromAsset("Textures/RedBlock.png");
		textures[4].loadFromAsset("Textures/YellowBlock.png");
	}

	void initParamecia() {
		for (std::size_t i = 0; i < paramecia.size(); i++) {
			sb::Texture* texture = &textures[rand() % textures.size()];
			paramecia[i].setTexture(texture);
		}
	}

	Scene10()
		: textures(5), paramecia(30)
	{
		initTextures();
		initParamecia();
	}

	void update(float ds) {
		for (std::size_t i = 0; i < paramecia.size(); i++)
			paramecia[i].update(ds);
	}

	void draw(sb::DrawTarget& target) {
		for (std::size_t i = 0; i < paramecia.size(); i++)
			paramecia[i].draw(target);
	}
};

float getDeltaSeconds() {
	static sb::Stopwatch sw;
	float deltaSeconds = sw.getElapsedSeconds();
	sw.reset();
	return deltaSeconds;
}

void demo10() {
	sb::Window window;
	Scene10 scene;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		scene.update(getDeltaSeconds());

		window.clear(sb::Color(0, 0, 0, 1));
		scene.draw(window);
		window.display();
	}
}

void printStats()
{
	static sb::Stopwatch sw;
	static std::size_t i = 0;
	static std::size_t numFrames = 0;

	if (i % 100 == 0) {
		float fps = float(numFrames) / sw.getElapsedSeconds();
		numFrames = 0;
		sw.reset();
		SDL_Log("Num Draw Calls: %d, FPS: %f", sb::Renderer::getNumDrawCalls(), fps);
	}

	i++;
	numFrames++;
	sb::Renderer::resetStatistics();
}

void demo11() {
	sb::Window window;
	sb::DrawBatch batch;
	sb::Texture texture1;
	sb::Texture texture2;
	texture1.loadFromAsset("Textures/RedBlock.png");
	texture2.loadFromAsset("Textures/GreenBlock.png");
	sb::Sprite sprite1;
	sb::Sprite sprite2;
	sprite1.setPosition(-0.5f, -0.5f);
	sprite1.setTexture(&texture1);
	sprite2.setTexture(&texture2);
	sprite2.setPosition(0.5f, 0.5f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(0, 0, 0, 1));
		batch.draw(sprite1);
		batch.draw(sprite2);
		window.draw(batch);
		window.display();

		printStats();
	}
}

class Scene12 : public sb::Drawable {
	std::vector<sb::Texture> textures;
	std::vector<sb::Sprite> sprites;

protected:
	void initTextures() {
		textures[0].loadFromAsset("Textures/CyanBlock.png");
		textures[1].loadFromAsset("Textures/GreenBlock.png");
		textures[2].loadFromAsset("Textures/PurpleBlock.png");
		textures[3].loadFromAsset("Textures/RedBlock.png");
		textures[4].loadFromAsset("Textures/YellowBlock.png");
	}

	void init(sb::Sprite& sprite) {
		sprite.setPosition(sb::random2D(-1, 1));
		sprite.setRotation(sb::random(0, 2 * sb::Pi));
		sprite.setScale(sb::random(0.015f, 0.07f));
		sprite.setTexture(&textures[rand() % textures.size()]);
	}

	static bool compare(sb::Sprite& left, sb::Sprite& right) {
		return left.getTexture() < right.getTexture();
	}

	void sort() {
		std::sort(sprites.begin(), sprites.end(), compare);
	}

public:
	Scene12()
		: textures(5), sprites(5000)
	{
		initTextures();

		for (std::size_t i = 0; i < sprites.size(); i++)
			init(sprites[i]);
		sort();
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		for (std::size_t i = 0; i < sprites.size(); i++)
			target.draw(sprites[i]);
	}
};

void demo12() {
	sb::Window window;
	sb::DrawBatch batch(16384);
	Scene12 scene;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(0, 0, 0, 1));
		batch.draw(scene);
		window.draw(batch);
		//scene.draw(window);
		window.display();

		printStats();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Texture Renderer: Build %s %s", __DATE__, __TIME__);

	srand(43);

	demo12();
	
	//demo11();

	// demo10();

	// demo9();

	//demo8();

	// demo8();

	// demo7();

	//demo6();

	//demo5();

	//demo4();

	//demo3();

	//demo2();

	//demo1();
}
