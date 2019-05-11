#include "DrawBatch.h"
#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Logger.h"
#include "Math.h"
#include <vector>
#include <algorithm>

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	static sb::Stopwatch sw;
	float elapsed = sw.getElapsedSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void demo0() {
	sb::Window window(300, 600);
	window.getCamera().setWidth(0.75f);

	sb::Texture texture;
	texture.enableMipmaps(true);
	texture.loadFromAsset("Textures/CoordinateSystem.png");

	sb::Sprite greenBlock;
	greenBlock.setTexture(&texture);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(greenBlock);
		window.display();
	}
}

bool nearby(const sb::Vector2f& first, const sb::Vector2f& second) {
	return (first - second).getLength() < 0.001f;
}

sb::Vector2f moveTowards(const sb::Vector2f& current, const sb::Vector2f& target, float ds) {
	sb::Vector2f distance = target - current;
	float distanceLength = distance.getLength();

	if (ds > distanceLength)
		return target;

	return current + ds * distance.normalized();
}

void patrol(sb::Vector2f& currentPosition, std::size_t& currentTargetIndex, const std::vector<sb::Vector2f>& targets, float ds) {
	if (nearby(currentPosition, targets[currentTargetIndex]))
		currentTargetIndex = (currentTargetIndex + 1) % targets.size();

	currentPosition = moveTowards(currentPosition, targets[currentTargetIndex], ds);
}

void patrol(sb::Transformable& entity, std::size_t& currentTargetIndex, const std::vector<sb::Vector2f>& targets, float ds) {
	sb::Vector2f position = entity.getPosition();
	patrol(position, currentTargetIndex, targets, ds);
	entity.setPosition(position);
}

void patrolEntity(sb::Transformable& entity, float ds) {
	static std::vector<sb::Vector2f> targets = { sb::Vector2f(-0.5f, -0.5f), sb::Vector2f(0.5f, 0.5f) };
	static std::size_t currentTarget = 0;
	patrol(entity, currentTarget, targets, ds);
}

void patrolCamera(sb::Camera& camera, std::size_t& currentTargetIndex, const std::vector<sb::Vector2f>& targets, float ds) {
	sb::Vector2f position = camera.getPosition();
	patrol(position, currentTargetIndex, targets, ds);
	camera.setPosition(position);
}

void update1(sb::Window& window, float ds) {
	window.getCamera().rotate(ds);

	static std::vector<sb::Vector2f> targets = { sb::Vector2f(-0.5f, -0.5f), sb::Vector2f(0.5f, 0.5f) };
	static std::size_t currentTarget = 0;
	patrolCamera(window.getCamera(), currentTarget, targets, ds);
}

void demo1() {
	sb::Window window(300, 600);

	sb::Texture texture;
	texture.enableMipmaps(true);
	texture.loadFromAsset("Textures/ReferenceQuad.png");

	sb::Sprite entity;
	entity.setTexture(&texture);
	entity.setScale(0.5f, 1);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		update1(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(entity);
		window.display();
	}
}

void update2(sb::Window& window, float ds) {
	static const float MinWidth = 0.25f;
	static const float MaxWidth = 4;
	static bool isGrowing = true;

	float delta = isGrowing ? ds : -ds;
	float newWidth = window.getCamera().getWidth() + delta;
	window.getCamera().setWidth(newWidth);

	if (isGrowing && newWidth > MaxWidth)
		isGrowing = false;
	else if (!isGrowing && newWidth < MinWidth)
		isGrowing = true;
}

void demo2() {
	sb::Window window(300, 600);

	sb::Texture texture;
	texture.enableMipmaps(true);
	texture.loadFromAsset("Textures/CoordinateSystem.png");

	sb::Sprite entity;
	entity.setTexture(&texture);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		update2(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(entity);
		window.display();
	}
}

void update3(sb::Window& window, float ds) {
	static int state = 0;

	if (sb::Input::isTouchGoingDown(1))
		state = (state + 1) % 3;

	if (state == 0) {
		window.getCamera().rotate(ds);
	}
	else if (state == 1) {
		static std::vector<sb::Vector2f> targets = { sb::Vector2f(-0.5f, 0), sb::Vector2f(0.5f, 0) };
		static std::size_t currentTarget = 0;
		patrolCamera(window.getCamera(), currentTarget, targets, ds);
	}
}

void demo3() {
	sb::DrawBatch batch;
	sb::Window window(300, 600);

	sb::Texture referenceTex;
	sb::Texture blockTex;
	sb::Sprite referenceQuad;
	std::vector<sb::Sprite> entities(4);

	referenceTex.loadFromAsset("Textures/ReferenceQuad.png");
	blockTex.enableMipmaps(true);
	blockTex.loadFromAsset("Textures/GreenBlock.png");

	referenceQuad.setTexture(&referenceTex);
	std::vector<sb::Vector2f> positions =
		{ sb::Vector2f(-0.25f, -0.25f), sb::Vector2f( 0.25f, -0.25f), 
		  sb::Vector2f(-0.25f,  0.25f), sb::Vector2f( 0.25f,  0.25f) };
	for (std::size_t i = 0; i < entities.size(); i++) {
		entities[i].setTexture(&blockTex);
		entities[i].setScale(0.3f, 0.3f);
		entities[i].setPosition(positions[i]);
	}

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		update3(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(referenceQuad);
		for (std::size_t i = 0; i < entities.size(); i++)
			batch.draw(entities[i]);
		window.draw(batch);
		window.display();
	}
}

int main(int argc, char* args[])
{
	demo3();

	// demo2();

	// demo1();

	// demo0();
}
