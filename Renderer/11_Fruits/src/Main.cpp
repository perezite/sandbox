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
		float ds = getDeltaSeconds();
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
		: debug(false), boundingCircle(50, 0.03f)
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
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(apple);
		window.display();
	}
}

/* class Scene0 : public sb::Drawable {
	sb::DrawBatch batch;
	std::vector<sb::Texture> textures;
	std::vector<Fruit> fruits;

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

	void initFruits(std::size_t numFruits) {
		std::size_t textureIndex = 0;
		for (std::size_t i = 0; i < numFruits; i++) {
			fruits[i].setPosition(sb::random2D(-1, 1));
			float scale = sb::random(0.1f, 0.15f);
			fruits[i].setScale(scale, scale);
			fruits[i].setRotation(sb::random(2.0f * sb::Pi));
			fruits[i].setTexture(&textures[textureIndex]);
			textureIndex = ++textureIndex % textures.size();
		}
	}

public:
	Scene0(std::size_t numFruits)
		: batch(8192), textures(9), fruits(numFruits)
	{
		initTextures();
		initFruits(numFruits);
	}

	void update(float ds) {
		for (std::size_t i = 0; i < fruits.size(); i++) {
			fruits[i].update(ds);
		}
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault()) {
		for (std::size_t i = 0; i < fruits.size(); i++)
			batch.draw(fruits[i]), drawStates;
		target.draw(batch);
	}
}; */

int main(int argc, char* args[])
{
	SDL_Log("Fruits Renderer: Build %s %s", __DATE__, __TIME__);
	
	demo1();

	//demo0();
}
