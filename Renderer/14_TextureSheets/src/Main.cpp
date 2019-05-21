#include "DrawBatch.h"
#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "TextureSheet.h"
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
	texture.loadFromAssetIntoSubArea("Textures/32x32.png", sb::Vector2i(5, 10));
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

void demo5() {
	sb::DrawBatch batch;
	sb::Window window(300, 600);
	sb::TextureSheet sheet(sb::Vector2i(242, 242));
	sb::Sprite greenSprite;
	sb::Sprite yellowSprite;
	sb::Sprite purpleSprite;
	sb::Sprite redSprite;

	sheet.loadFromAsset("Textures/GreenBlock.png", sb::Vector2i(0, 0));
	sheet.loadFromAsset("Textures/YellowBlock.png", sb::Vector2i(121, 0));
	sheet.loadFromAsset("Textures/PurpleBlock.png", sb::Vector2i(0, 121));
	sheet.loadFromAsset("Textures/RedBlock.png", sb::Vector2i(121, 121));

	greenSprite.setTexture(sheet.getTexture(), sheet.getArea(0));
	greenSprite.setScale(0.2f);
	greenSprite.setPosition(-0.2f, -0.2f);
	yellowSprite.setTexture(sheet.getTexture(), sheet.getArea(1));
	yellowSprite.setScale(0.2f);
	yellowSprite.setPosition(0.2f, -0.2f);
	purpleSprite.setTexture(sheet.getTexture(), sheet.getArea(2));
	purpleSprite.setScale(0.2f);
	purpleSprite.setPosition(-0.2f, 0.2f);
	redSprite.setTexture(sheet.getTexture(), sheet.getArea(3));
	redSprite.setScale(0.2f);
	redSprite.setPosition(0.2f, 0.2f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(greenSprite);
		window.draw(yellowSprite);
		window.draw(purpleSprite);
		window.draw(redSprite);
		window.display();

		SB_MESSAGE("Draw calls: " << sb::Renderer::getNumDrawCalls());
		sb::Renderer::resetStatistics();
	}
}

void demo6() {
	sb::DrawBatch batch;
	sb::Window window(300, 600);
	sb::TextureSheet sheet(sb::Vector2i(242, 242));
	sb::Texture cyanTexture;
	sb::Sprite greenSprite;
	sb::Sprite yellowSprite;
	sb::Sprite purpleSprite;
	sb::Sprite redSprite;
	sb::Sprite cyanSprite;

	sheet.loadFromAsset("Textures/GreenBlock.png", sb::Vector2i(0, 0));
	sheet.loadFromAsset("Textures/YellowBlock.png", sb::Vector2i(121, 0));
	sheet.loadFromAsset("Textures/PurpleBlock.png", sb::Vector2i(0, 121));
	sheet.loadFromAsset("Textures/RedBlock.png", sb::Vector2i(121, 121));
	cyanTexture.loadFromAsset("Textures/CyanBlock.png");


	greenSprite.setTexture(sheet.getTexture(), sheet.getArea(0));
	greenSprite.setScale(0.2f);
	greenSprite.setPosition(-0.2f, -0.2f);
	yellowSprite.setTexture(sheet.getTexture(), sheet.getArea(1));
	yellowSprite.setScale(0.2f);
	yellowSprite.setPosition(0.2f, -0.2f);
	purpleSprite.setTexture(sheet.getTexture(), sheet.getArea(2));
	purpleSprite.setScale(0.2f);
	purpleSprite.setPosition(-0.2f, 0.2f);
	redSprite.setTexture(sheet.getTexture(), sheet.getArea(3));
	redSprite.setScale(0.2f);
	redSprite.setPosition(0.2f, 0.2f);
	cyanSprite.setTexture(cyanTexture);
	cyanSprite.setScale(0.3f, 0.3f);
	cyanSprite.setRotation(30 * sb::ToRadian);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		batch.draw(greenSprite);
		batch.draw(yellowSprite);
		batch.draw(purpleSprite);
		batch.draw(redSprite);
		batch.draw(cyanSprite);
		batch.draw(cyanSprite);
		window.draw(batch);
		window.display();

		SB_MESSAGE("Draw calls: " << sb::Renderer::getNumDrawCalls());
		sb::Renderer::resetStatistics();
	}
}

void demo7() {
	sb::DrawBatch batch;
	sb::Window window(300, 600);
	sb::TextureSheet sheet(sb::Vector2i(172, 104));
	std::vector<sb::Texture> textures(4);
	std::vector<sb::Sprite> sprites(4);
	std::vector<sb::Sprite> referenceSprites(4);
	sb::Sprite debugSprite;
	std::size_t index = -1;

	// TODO: If I load the single textures first, I get a GL error :/
	//textures[0].loadFromAsset("Textures/BleedingTestBlue.png");
	//textures[1].loadFromAsset("Textures/BleedingTestRed.bmp");
	//textures[2].loadFromAsset("Textures/BleedingTestGreen.png");
	//textures[3].loadFromAsset("Textures/BleedingTestYellow.bmp");
	sheet.loadFromAsset("Textures/BleedingTestBlue.png", sb::Vector2i(0, 0));
	sheet.loadFromAsset("Textures/BleedingTestRed.bmp", sb::Vector2i(64, 0));
	sheet.loadFromAsset("Textures/BleedingTestGreen.png", sb::Vector2i(0, 64));
	sheet.loadFromAsset("Textures/BleedingTestYellow.bmp", sb::Vector2i(132, 64));
	textures[0].loadFromAsset("Textures/BleedingTestBlue.png");
	textures[1].loadFromAsset("Textures/BleedingTestRed.bmp");
	textures[2].loadFromAsset("Textures/BleedingTestGreen.png");
	textures[3].loadFromAsset("Textures/BleedingTestYellow.bmp");

	sprites[0].setPosition(0, 0.5f);
	sprites[0].setScale(0.9f);
	sprites[0].setTexture(sheet.getTexture(), sheet.getArea(0));
	sprites[1].setPosition(0, 0.5f);
	sprites[1].setScale(0.9f);
	sprites[1].setTexture(sheet.getTexture(), sheet.getArea(1));
	sprites[2].setPosition(0, 0.5f);
	sprites[2].setScale(0.9f);
	sprites[2].setTexture(sheet.getTexture(), sheet.getArea(2));
	sprites[3].setPosition(0, 0.5f);
	sprites[3].setScale(0.9f);
	sprites[3].setTexture(sheet.getTexture(), sheet.getArea(3));

	referenceSprites[0].setPosition(0, -0.5f);
	referenceSprites[0].setScale(0.9f);
	referenceSprites[0].setTexture(textures[0]);
	referenceSprites[1].setPosition(0, -0.5f);
	referenceSprites[1].setScale(0.9f);
	referenceSprites[1].setTexture(textures[1]);
	referenceSprites[2].setPosition(0, -0.5f);
	referenceSprites[2].setScale(0.9f);
	referenceSprites[2].setTexture(textures[2]);
	referenceSprites[3].setPosition(0, -0.5f);
	referenceSprites[3].setScale(0.9f);
	referenceSprites[3].setTexture(textures[3]);

	debugSprite.setTexture(sheet.getTexture());
	debugSprite.setScale(1, (float)sheet.getTexture().getSize().y / (float)sheet.getTexture().getSize().x);

	while (window.isOpen()) {
		sb::Input::update();
		if (sb::Input::isTouchGoingDown(1)) 
			index = (index + 1) % sprites.size();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		if (index == -1) {
			window.draw(debugSprite);
		} else {
			window.draw(sprites[index]);
			window.draw(referenceSprites[index]);
		}
		window.display();
	}
}

int main() {
	demo7();

	//demo6();

	//demo5();

	//demo4();

	//demo3();

	// demo2();

	// demo1();

	// demo0();

	// concept0();

	std::cin.get();
	return 0;
}