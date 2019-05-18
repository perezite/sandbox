#include "DrawBatch.h"
#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Color.h"
#include "Vector2.h"
#include <iostream>
#include <vector>

void drawSprites(sb::DrawBatch& batch, std::vector<sb::Sprite>& sprites) {
	for (std::size_t i = 0; i < sprites.size(); i++)
		batch.draw(sprites[i]);
}

void concept0() {
	sb::DrawBatch batch;
	sb::Window window;

	// sb::TextureSheet blockSheet(242, 242); // default = max size = 1024 * 1024
	// blockSheet.loadFromAsset("Textures/CyanBlock.png", 0, 0);		
	// blockSheet.loadFromAsset("Textures/GreenBlock.png", sb::Vector2i(121, 0));
	// blockSheet.loadFromAsset("Textures/PurpleBlock.png", 0, 121);		
	// blockSheet.loadFromAsset("Textures/RedBlock.png", sb::Vector2i(121, 121));

	std::vector<sb::Sprite> sprites(4);
	// sprites[0].setTexture(blockSheet.getTexture(), 0, 0, 121, 121);
	// sprites[1].setTexture(blockSheet.getTexture(), sb::IntRect(121, 0, 121, 121))
	// sprites[2].setTexture(blockSheet.getTexture(), blockTextures[2]);
	// sprites[3].setTexture(blockSheet.getTexture(), blockTextures[3]);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		drawSprites(batch, sprites);
		window.draw(batch);

		window.clear();
		window.display();
	}
}

void demo0() {
	sb::Window window;
	sb::Texture texture;
	sb::Sprite sprite;

	texture.loadFromAsset("Textures/CyanBlock.png");	
	sprite.setTexture(texture);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear();
		window.draw(sprite);
		window.display();
	}
}

void demo1() {
	sb::Window window;
	sb::Vector2i v1(1, 2);
	sb::Vector2i v2(3, 4);
	sb::Vector2i v3 = 3 * v1 - v2;
	sb::Vector2f test(1, 2);
	v3 += -v2;
	std::cout << v3.x << " " << v3.y << std::endl;
	std::cout << test.normalized().x << " " << test.normalized().y << std::endl;
	
	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.display();
	}
}

void demo2() {
	sb::Window window(300, 600);
	sb::Texture texture;
	sb::Sprite sprite;
	
	texture.loadFromAsset("Textures/GreenBlock.png");
	sprite.setTexture(texture);

	while (window.isOpen()) {
		sb::Input::update();
		if (sb::Input::isTouchGoingDown(1))
			texture.enableMipmap(!texture.isMipmapEnabled());

		window.update();

		window.clear();
		window.draw(sprite);
		window.display();
	}
}

void demo3() {
	sb::Window window(300, 600);
	sb::Texture texture;
	sb::Sprite sprite;

	texture.loadFromAsset("Textures/48x48.png");
	texture.loadFromAssetIntoSubArea("Textures/32x32.png", sb::Vector2i(10, 10));
	sprite.setTexture(texture);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear();
		window.draw(sprite);
		window.display();
	}
}

void demo4() {
	sb::Window window(300, 600);
	sb::Texture referenceTexture;
	sb::Texture blockTexture;
	sb::Sprite referenceSprite;
	sb::Sprite blockSprite;

	referenceTexture.loadFromAsset("Textures/48x48.png");
	referenceTexture.loadFromAssetIntoSubArea("Textures/32x32.png", sb::Vector2i(5, 10));
	blockTexture.loadFromAsset("Textures/GreenBlock.png");
	referenceSprite.setTexture(referenceTexture, sb::IntRect(4, 11, 32, 32));
	blockSprite.setTexture(blockTexture);
	blockSprite.setScale(0.2f, 0.2f);
	blockSprite.setPosition(-0.2f, -0.2f);


	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear();
		 window.draw(referenceSprite);
		window.draw(blockSprite);
		window.display();
	}
}

int main() {
	
	demo4();

	// demo3();

	// demo2();

	// demo1();

	// demo0();

	// concept0();

	std::cin.get();
	return 0;
}