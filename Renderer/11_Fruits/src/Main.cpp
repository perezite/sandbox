#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Texture.h"
#include "Sprite.h"
#include "Stopwatch.h"
#include "Quad.h"
#include "Math.h"
#include "Physics.h"
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

class DebugCircle : public sb::Drawable, public sb::Transformable {
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

class Fruit : public sb::Drawable, public sb::Transformable {
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

class Scene2 : public sb::Drawable, public sb::Transformable {
	sb::DrawBatch batch;
	std::vector<sb::Texture> textures;
	std::vector<Fruit> fruits;
	std::vector<sb::Vector2f> fruitVelocities;
	std::vector<sb::Vector2f> fruitForces;
	float dragCoefficient;
	float inverseAspect;

protected:
	void initTextures() {
		textures[0].loadFromAsset("Textures/coconut.png");
		textures[1].loadFromAsset("Textures/garlic.png");
		textures[2].loadFromAsset("Textures/grape.png");
		textures[3].loadFromAsset("Textures/kiwi.png");
		textures[4].loadFromAsset("Textures/lemon.png");
		textures[5].loadFromAsset("Textures/lettuce.png");
		textures[6].loadFromAsset("Textures/peach.png");
		textures[7].loadFromAsset("Textures/raspberry.png");
		textures[8].loadFromAsset("Textures/tomato.png");
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

		const sb::Vector2f& touchPosition = sb::Input::getTouchPosition(window);
		std::cout << touchPosition.x << " " << touchPosition.y << std::endl;

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
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
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
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
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			scene.getFruits()[0].setPosition(touch);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}

class Physics {
	float _inverseAspect;
	float _dragCoefficient;
	float _fixedDeltaSeconds;
	std::vector<sb::Body*> _bodies;
	std::vector<sb::Vector2f> _forces;

protected:
	void prepare() {
		_forces.clear();
		_forces.resize(_bodies.size());
	}
	
	const sb::Vector2f computeCollisionForce(const sb::Body& current, const sb::Body& other) {
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
		for (std::size_t i = 0; i < _bodies.size(); i++) {
			for (std::size_t j = i + 1; j < _bodies.size(); j++) {
				const sb::Body& current = *_bodies[i];
				const sb::Body& other = *_bodies[j];
				const sb::Vector2f& force = computeCollisionForce(current, other);
				_forces[i] += force;
				_forces[j] += -force;
			}
		}
	}

	sb::Vector2f computeBoundaryForce(const sb::Body& fruit) {
		const float radius = (fruit.getScale().x + fruit.getScale().y) / 4.0f;
		const sb::Vector2f position = fruit.getPosition();
		sb::Vector2f force;

		float left = position.x - radius;
		float top = position.y + radius;
		float right = position.x + radius;
		float bottom = position.y - radius;

		if (left < -1)
			force += (-1 - left) * sb::Vector2f(1, 0);
		if (top > 1 * _inverseAspect)
			force += (top - 1 * _inverseAspect) * sb::Vector2f(0, -1);
		if (right > 1)
			force += (right - 1) * sb::Vector2f(-1, 0);
		if (bottom < -1 * _inverseAspect)
			force += (-1 * _inverseAspect - bottom) * sb::Vector2f(0, 1);

		return 1000 * force;
	}

	void computeBoundaryForces() {
		for (std::size_t i = 0; i < _bodies.size(); i++)
			_forces[i] += computeBoundaryForce(*_bodies[i]);
	}


	void computeDragForces() {
		for (std::size_t i = 0; i < _bodies.size(); i++)
			_forces[i] -= _dragCoefficient * _bodies[i]->velocity;
	}

	void computeForces() {
		computeCollisionForces();
		computeBoundaryForces();
		computeDragForces();
	}

	void moveBodies(sb::Body& body, const sb::Vector2f& force, float ds) {
		body.velocity += ds * force;
		sb::Vector2f position = body.getPosition();
		position += ds * body.velocity;
		body.setPosition(position);
	}

	void moveBodies(float ds) {
		for (std::size_t i = 0; i < _bodies.size(); i++)
			moveBodies(*_bodies[i], _forces[i], ds);
	}

	void step(float ds) {
		prepare();
		computeForces();
		moveBodies(ds);
		_bodies.clear();
	}

public:
	Physics(float aspect)
		: _inverseAspect(1 / aspect), _dragCoefficient(10), _fixedDeltaSeconds(0.02f)
	{ }

	inline void setDragCoefficient(float drag) { _dragCoefficient = drag; }

	void addBody(sb::Body& body) {
		_bodies.push_back(&body);
	}

	void simulate(float ds) {
		float sum = 0;
		float remaining = ds;
		do {
			float timestep = std::min(remaining, _fixedDeltaSeconds);
			step(timestep);
			remaining -= _fixedDeltaSeconds;
			sum += timestep;
		} while (remaining > 0);
	}
};

class Fruit2 : public sb::Drawable, public sb::Body {
	sb::Sprite sprite;
	bool debug;
	DebugCircle boundingCircle;

public:
	Fruit2()
		: debug(false), boundingCircle(50, 0.1f)
	{
		boundingCircle.setScale(0.5f, 0.5f);
	}

	inline void setTexture(sb::Texture* texture) { sprite.setTexture(texture); }

	inline void setDebug(bool enable) { debug = enable; }

	void update(float ds) {
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();
		target.draw(sprite, drawStates);
		if (debug)
			target.draw(boundingCircle, drawStates);
	}
};

void init7(std::vector<Fruit2>& fruits, std::vector<sb::Texture>& textures) {
	fruits[0].setPosition(-0.5f, -0.5f);
	fruits[0].setScale(0.3f, 0.3f);
	fruits[0].setTexture(&textures[0]);
	fruits[1].setPosition(0.5f, 0.5f);
	fruits[1].setScale(0.3f, 0.3f);
	fruits[1].setTexture(&textures[1]);
}

void update7(std::vector<Fruit2>& fruits, sb::Window& window) {
	if (sb::Input::isTouchDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		fruits[0].setPosition(touch);
	}
}

void draw7(std::vector<Fruit2>& fruits, sb::Window& window) {
	for (std::size_t i = 0; i < fruits.size(); i++)
		window.draw(fruits[i]);
}

void demo7() {
	float width = 400;
	float height = 400;
	float aspect = width / height;
	sb::Window window((int)width, (int)height);
	Physics physics(aspect);
	physics.setDragCoefficient(8);

	std::vector<sb::Texture> textures(2);
	std::vector<Fruit2> fruits(2);
	textures[0].loadFromAsset("Textures/coconut.png");
	textures[1].loadFromAsset("Textures/garlic.png");
	init7(fruits, textures);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		update7(fruits, window);
		for (std::size_t i = 0; i < fruits.size(); i++)
			physics.addBody(fruits[i]);
		 physics.simulate(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		draw7(fruits, window);
		window.display();
	}
}

class Scene8 : public sb::Drawable, public sb::Transformable {
	sb::DrawBatch batch;
	sb::Physics physics;
	std::vector<sb::Texture> textures;
	std::vector<Fruit2> fruits;
	float inverseAspect;

protected:
	void initTextures() {
		textures[0].loadFromAsset("Textures/coconut.png");
		textures[1].loadFromAsset("Textures/garlic.png");
		textures[2].loadFromAsset("Textures/grape.png");
		textures[3].loadFromAsset("Textures/kiwi.png");
		textures[4].loadFromAsset("Textures/lemon.png");
		textures[5].loadFromAsset("Textures/lettuce.png");
		textures[6].loadFromAsset("Textures/peach.png");
		textures[7].loadFromAsset("Textures/raspberry.png");
		textures[8].loadFromAsset("Textures/tomato.png");
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

public:
	Scene8(sb::Physics& physics_, std::size_t numFruits, const sb::Vector2f& fruitScaleRange, float aspect_)
		: batch(8192), physics(physics_), textures(9), fruits(numFruits), inverseAspect(1 / aspect_)
	{
		initTextures();
		initFruits(numFruits, fruitScaleRange);
	}

	inline std::vector<Fruit2>& getFruits() { return fruits; };

	void setDebug(bool debug) {
		for (std::size_t i = 0; i < fruits.size(); i++)
			fruits[i].setDebug(debug);
	}

	void update(float ds, sb::Window& window) {
		for (std::size_t i = 0; i < fruits.size(); i++)
			physics.addBody(fruits[i]);
		physics.simulate(ds, window);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		drawStates.transform *= getTransform();

		for (std::size_t i = 0; i < fruits.size(); i++)
			batch.draw(fruits[i], drawStates);
		target.draw(batch);
	}
};

void demo8() {
	sb::Window window(360, 640);
	sb::Physics physics(window.getAspect());
	physics.setDragCoefficient(8);
	Scene8 scene(physics, 100, sb::Vector2f(0.05f, 0.3f), window.getAspect());
	scene.getFruits()[0].setScale(0.35f, 0.35f);
	scene.setScale(1, window.getAspect());

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		scene.update(ds, window);
		if (sb::Input::isTouchDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			scene.getFruits()[0].setPosition(touch);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(scene);
		window.display();
	}
}

void testLog(std::stringstream& stream) {
	std::cout << stream.rdbuf();
}

#define TEST_LOG(stream) do {						\
	std::stringstream test;							\
	test << stream << std::endl;					\
	testLog(test);									\
} while (false)

void demo9() {
	TEST_LOG("test" << " " << "test2");
	TEST_LOG("test3");
	std::cin.get();
}

void demo10() {
	SB_MESSAGE("just some " << "message");
	SB_MESSAGE_IF(false, "this message should not" << " be logged");
	SB_MESSAGE_IF(true, "this message " << "should be logged");
	SB_DEBUG("this" << " should only be logged in debug mode");
	SB_DEBUG_IF(false, "this" << " debug message should never be logged");
	SB_DEBUG_IF(true, "this" << " debug message should only be logged in debug mode");
	SB_WARNING2("uhhh, " << " danger!");
	SB_WARNING_IF2(false, "this " << "warning should not be logged");
	SB_WARNING_IF2(true, "this " << "warning should be logged");
	SB_ERROR_IF2(false, "this error should not be " << "logged");
	SB_ERROR_IF2(true, "this error should be" << " logged");
	SB_ERROR2("ohoh " << "an error!");

	std::cin.get();
}

void demo11() {
	// provoke error
	sb::Window window;
	sb::Texture texture;
	texture.loadFromAsset("this_file_does_not_exist.png");
}

int main(int argc, char* args[])
{
	SDL_Log("Fruits Renderer: Build %s %s", __DATE__, __TIME__);

	srand(987654321);

	//demo11();

	demo10();

	//demo9();

	 //demo8();

	// demo7();

	// demo6();

	// demo5();

	// demo4();

	// demo3();

	// demo2();

	// demo1();

	// demo0();
}
