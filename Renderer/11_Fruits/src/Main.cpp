#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Texture.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Quad.h"
#include "Math.h"
#include <vector>

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	static sb::Stopwatch sw;
	float elapsed = sw.getElapsedSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

class DebugCircle : public sb::Drawable {
	sb::Mesh mesh;

protected:
	void initSegments(std::size_t numSegments, float thickness) {
		sb::Color color(1, 0, 0, 1);
		float angleSegment = 2.0f * sb::Pi / float(numSegments);
		for (std::size_t i = 0; i < numSegments; i++) {
			float startAngle = float(i) * angleSegment;
			float endAngle = startAngle + angleSegment;
			sb::Vector2f startPoint(cosf(startAngle), sinf(startAngle));
			sb::Vector2f endPoint(cosf(endAngle), sinf(endAngle));
			sb::Vector2f startPointInner = (1 - 0.5f * thickness) * startPoint;
			sb::Vector2f startPointOuter = (1 + 0.5f * thickness) * startPoint;
			sb::Vector2f endPointInner = (1 - 0.5f * thickness) * endPoint;
			sb::Vector2f endPointOuter = (1 + 0.5f * thickness) * endPoint;
			mesh[i * 6 + 0] = sb::Vertex(startPointInner, color);
			mesh[i * 6 + 1] = sb::Vertex(startPointInner, color);
			mesh[i * 6 + 2] = sb::Vertex(startPointOuter, color);
			mesh[i * 6 + 3] = sb::Vertex(endPointInner, color);
			mesh[i * 6 + 4] = sb::Vertex(endPointOuter, color);
			mesh[i * 6 + 5] = sb::Vertex(endPointOuter, color);
		}
	}

public:
	DebugCircle(std::size_t numSegments, float thickness)
		: mesh(numSegments * 6, sb::PrimitiveType::TriangleStrip)
	{
		initSegments(numSegments, thickness);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates) {
		drawStates.transform *= getTransform();
		target.draw(mesh.getVertices(), mesh.getPrimitiveType(), drawStates);
	}
};

void demo0() {
	sb::Window window;
	DebugCircle circle(50, 0.03f);
	circle.setPosition(-0.5f, -0.5f);
	circle.setScale(0.2f, 0.2f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(circle);
		window.display();
	}
}

class Fruit : public sb::Drawable {
	sb::Sprite sprite;
	bool debug;
	DebugCircle boundingCircle;

public:
	Fruit()
		: debug(false), boundingCircle(50, 0.1f)
	{
		boundingCircle.setScale(0.5f, 0.5f);
	}

	inline void setTexture(sb::Texture* texture) { sprite.setTexture(texture); }

	inline void isDebug(bool enable) { debug = enable; }

	void update(float ds) {
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();
		target.draw(sprite, drawStates);
		if (debug)
			target.draw(boundingCircle, drawStates);
	}
};

void demo1() {
	sb::Window window;

	sb::Texture appleTexture("Textures/lemon.png");
	Fruit apple;
	apple.setTexture(&appleTexture);
	apple.isDebug(true);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(apple);
		window.display();
	}
}

class Scene2 : public sb::Drawable {
	sb::DrawBatch batch;
	std::vector<sb::Texture> textures;
	std::vector<Fruit> fruits;
	std::vector<sb::Vector2f> fruitVelocities;
	std::vector<sb::Vector2f> fruitForces;
	float dragCoefficient;
	float inverseAspect;

protected:
	void initTextures() {
		textures[0].loadFromAsset("Textures/apple.png");
		textures[1].loadFromAsset("Textures/bananas.png");
		textures[2].loadFromAsset("Textures/carrot.png");
		textures[3].loadFromAsset("Textures/grapefruit.png");
		textures[4].loadFromAsset("Textures/lemon.png");
		textures[5].loadFromAsset("Textures/pumpkin.png");
		textures[6].loadFromAsset("Textures/pineapple.png");
		textures[7].loadFromAsset("Textures/strawberry.png");
		textures[8].loadFromAsset("Textures/watermelon.png");
	}

	void initFruits(std::size_t numFruits, const sb::Vector2f& scaleRange) {
		std::size_t textureIndex = 0;
		for (std::size_t i = 0; i < numFruits; i++) {
			fruits[i].setPosition(sb::random(-1, 1), inverseAspect * sb::random(-1, 1));
			float scale = sb::random(scaleRange.x, scaleRange.y);
			fruits[i].setScale(scale, scale);
			fruits[i].setRotation(sb::random(2.0f * sb::Pi));
			fruits[i].setTexture(&textures[textureIndex]);
			textureIndex = ++textureIndex % textures.size();
		}
	}

	void input(float ds) {
		if (sb::Input::isKeyDown(sb::KeyCode::Left)) {
			fruits[0].translate(-1 * ds, 0);
			fruitVelocities[0] = sb::Vector2f(0, 0);
		}
		if (sb::Input::isKeyDown(sb::KeyCode::Up)) {
			fruits[0].translate(0, 1 * ds);
			fruitVelocities[0] = sb::Vector2f(0, 0);
		}
		if (sb::Input::isKeyDown(sb::KeyCode::Right)) {
			fruits[0].translate(1 * ds, 0);
			fruitVelocities[0] = sb::Vector2f(0, 0);
		}
		if (sb::Input::isKeyDown(sb::KeyCode::Down)) {
			fruits[0].translate(0, -1 * ds);
			fruitVelocities[0] = sb::Vector2f(0, 0);
		}
	}

	void clearForces() {
		for (std::size_t i = 0; i < fruitForces.size(); i++)
			fruitForces[i] = sb::Vector2f(0, 0);
	}

	const sb::Vector2f computeCollisionForce(const Fruit& current, const Fruit& other) {
		const sb::Vector2f currentScale = current.getScale();
		const sb::Vector2f otherScale = other.getScale();
		const float leftRadius = (currentScale.x + currentScale.y) / 4.0f;
		const float rightRadius = (otherScale.x + otherScale.y) / 4.0f;
		sb::Vector2f distance = other.getPosition() - current.getPosition();
		float distanceLength = distance.getLength();
		float collisionDistance = distanceLength - leftRadius - rightRadius;

		return collisionDistance < 0 ?
			collisionDistance * 1000 * distance.normalized() :
			sb::Vector2f(0, 0);
	}

	void computeCollisionForces() {
		for (std::size_t i = 0; i < fruits.size(); i++) {
			for (std::size_t j = i + 1; j < fruits.size(); j++) {
				const Fruit& current = fruits[i];
				const Fruit& other = fruits[j];
				const sb::Vector2f& force = computeCollisionForce(current, other);
				fruitForces[i] += force;
				fruitForces[j] += -force;
			}
		}
	}

	void computeDragForces() {
		for (std::size_t i = 0; i < fruits.size(); i++)
			fruitForces[i] -= dragCoefficient * fruitVelocities[i];
	}

	sb::Vector2f computeBoundaryForce(const Fruit& fruit) {
		const float radius = (fruit.getScale().x + fruit.getScale().y) / 4.0f;
		const sb::Vector2f position = fruit.getPosition();
		sb::Vector2f force;

		float left = position.x - radius;
		float top = position.y + radius;
		float right = position.x + radius;
		float bottom = position.y - radius;
		
		if (left < -1)
			force += (-1 - left) * sb::Vector2f(1, 0);
		if (top > 1 * inverseAspect)
			force += (top - 1 * inverseAspect) * sb::Vector2f(0, -1);
		if (right > 1)
			force += (right - 1) * sb::Vector2f(-1, 0);
		if (bottom < -1 * inverseAspect)
			force += (-1 * inverseAspect - bottom) * sb::Vector2f(0, 1);

		return 1000 * force;
	}

	void computeBoundaryForces() {
		for (std::size_t i = 0; i < fruitForces.size(); i++)
			fruitForces[i] += computeBoundaryForce(fruits[i]);
	}

	void integrate(float ds) {
		for (std::size_t i = 0; i < fruits.size(); i++) {
			fruitVelocities[i] += ds * fruitForces[i];
			sb::Vector2f position = fruits[i].getPosition();
			position += ds * fruitVelocities[i];
			fruits[i].setPosition(position);
		}
	}

	void physics(float ds) {
		clearForces();
		computeCollisionForces();
		computeBoundaryForces();
		computeDragForces();
		integrate(ds);
	}

public:
	Scene2(std::size_t numFruits, const sb::Vector2f& fruitScaleRange, float aspect_)
		: batch(8192), textures(9), fruits(numFruits), fruitVelocities(numFruits), 
		fruitForces(numFruits), dragCoefficient(10), inverseAspect(1 / aspect_)
	{
		initTextures();
		initFruits(numFruits, fruitScaleRange);
	}

	inline std::vector<Fruit>& getFruits() { return fruits; };

	void isDebug(bool debug) {
		for (std::size_t i = 0; i < fruits.size(); i++)
			fruits[i].isDebug(debug);
	}

	inline void setDragCoefficient(float coefficient) { dragCoefficient = coefficient; }

	void update(float ds) {
		input(ds);

		physics(ds);

		for (std::size_t i = 0; i < fruits.size(); i++)
			fruits[i].update(ds);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();

		for (std::size_t i = 0; i < fruits.size(); i++)
			batch.draw(fruits[i], drawStates);
		target.draw(batch);
	}
}; 

void demo2() {
	sb::Window window;

	Scene2 scene(100, sb::Vector2f(0.1f, 0.3f), 1);
	scene.isDebug(true);
	scene.getFruits()[0].setPosition(-0.3f, -0.3f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		scene.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}

void demo3() {
	sb::Window window;

	Scene2 scene(100, sb::Vector2f(0.05f, 0.3f), 1);
	scene.isDebug(false);
	scene.getFruits()[0].setPosition(-0.3f, -0.3f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		scene.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}

void demo4() {
	sb::Window window;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		const sb::Vector2f& touchPosition = sb::Input::getTouchPosition(window.getResolution());
		std::cout << touchPosition.x << " " << touchPosition.y << std::endl;

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

sb::Vector2f normalizePixelCoordinates(const sb::Vector2f& touchPosition, const sb::Vector2f& resolution, float aspect) {
	float inverseAspect = 1 / aspect;
	return sb::Vector2f(touchPosition.x * 2 / resolution.x - 1, touchPosition.y * 2 * inverseAspect / resolution.y - inverseAspect);
}

void demo5() {
	sb::Window window(800, 800);

	Scene2 scene(100, sb::Vector2f(0.05f, 0.3f), 1);
	scene.setDragCoefficient(8);
	scene.getFruits()[0].setScale(0.35f, 0.35f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		scene.update(ds);
		if (sb::Input::isTouchDown(1)) {
			sb::Vector2f touch = normalizePixelCoordinates(sb::Input::getTouchPosition(window.getResolution()), window.getResolution(), 1);
			 scene.getFruits()[0].setPosition(touch);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}

void demo6() {
	float width = 360 * 1.2f;
	float height = 640 * 1.2f;
	float aspect = width / height;
	sb::Window window((int)width, (int)height);

	Scene2 scene(100, sb::Vector2f(0.05f, 0.3f), aspect);
	scene.setDragCoefficient(8);
	scene.getFruits()[0].setScale(0.35f, 0.35f);
	scene.setScale(1, aspect);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		scene.update(ds);
		if (sb::Input::isTouchDown(1)) {
			sb::Vector2f touch = normalizePixelCoordinates(sb::Input::getTouchPosition(window.getResolution()), window.getResolution(), aspect);
			scene.getFruits()[0].setPosition(touch);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Fruits Renderer: Build %s %s", __DATE__, __TIME__);

	demo6();

	// demo5();

	// demo4();

	// demo3();

	// demo2();

	// demo1();

	// demo0();
}
