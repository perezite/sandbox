#include "DrawBatch.h"
#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Color.h"
#include "Vector2.h"
#include <iostream>
#include <vector>

// TODO
// implement templated vectors
// implement texture sheet class

void drawSprites(sb::DrawBatch& batch, std::vector<sb::Sprite>& sprites) {
	for (std::size_t i = 0; i < sprites.size(); i++)
		batch.draw(sprites[i]);
}

void concept0() {
	sb::DrawBatch batch;
	sb::Window window;

	// sb::TextureSheet blockSheet(242, 242); // default = max size = 1024 * 1024
	// blockSheet.addFromAsset("Textures/CyanBlock.png", 0, 0);		
	// blockSheet.addFromAsset("Textures/GreenBlock.png", sb::Vector2i(121, 0));
	// blockSheet.addFromAsset("Textures/PurpleBlock.png", 0, 121);		
	// blockSheet.addFromAsset("Textures/RedBlock.png", sb::Vector2i(121, 121));

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

void demo1() {
	sb::Window window;
	sb::Texture flatTexture;
	sb::Texture blockTexture;
	sb::Sprite flatSprite;
	sb::Sprite blockSprite;

	flatTexture.createEmpty(100, 100, sb::Color(0, 0, 0, 1));
	blockTexture.loadFromAsset("Textures/GreenBlock.png");
	blockTexture.enableMipmaps(true);

	flatSprite.setTexture(&flatTexture);
	flatSprite.setScale(0.5f, 0.5f);
	blockSprite.setTexture(&blockTexture);
	blockSprite.setScale(0.2f, 0.2f);
	blockSprite.setPosition(0.3f, 0.3f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1 , 1));
		window.draw(flatSprite);
		window.draw(blockSprite);
		window.display();
	}
}

void demo0() {
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

int main() {
	// demo1();

	demo0();

	// concept0();

	std::cin.get();
	return 0;
}