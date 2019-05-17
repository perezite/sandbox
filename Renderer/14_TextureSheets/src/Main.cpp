#include "DrawBatch.h"
#include "Window.h"
#include "Input.h"
#include "Sprite.h"
#include "Color.h"
#include <iostream>
#include <vector>

void drawSprites(sb::DrawBatch& batch, std::vector<sb::Sprite>& sprites) {
	for (std::size_t i = 0; i < sprites.size(); i++)
		batch.draw(sprites[i]);
}

void concept0() {
	sb::DrawBatch batch;
	sb::Window window;

	// sb::TextureSheet blockTextures(242, 242); // default = max size = 1024 * 1024
	// blockTextures.addFromAsset("Textures/CyanBlock.png", 0, 0, 121, 121);
	// blockTextures.addFromAsset("Textures/GreenBlock.png", sb::IntRect(121, 0, 121, 121));
	// blockTextures.addFromAsset("Textures/PurpleBlock.png", 0, 121);		
	// blockTextures.addFromAsset("Textures/RedBlock.png", sb::Vector2i(121, 121));
	
	std::vector<sb::Sprite> sprites(4);
	// sprites[0].setTexture(blockTextures, 0, 0, 121, 121);
	// sprites[1].setTexture(blockTextures, sb::IntRect(121, 0, 121, 121))
	// sprites[2].setTexture(blockTextures, blockTextures[2]);
	// sprites[3].setTexture(blockTextures, blockTextures[3]);

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
	sb::Texture flatTexture;
	sb::Texture blockTexture;
	sb::Sprite flatSprite;
	sb::Sprite blockSprite;

	flatTexture.createFromColor(100, 100, sb::Color(1, 0, 0, 0.5f));
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

		window.clear();
		window.draw(flatSprite);
		window.draw(blockSprite);
		window.display();
	}
}

int main() {
	demo0();

	// concept0();

	std::cin.get();
	return 0;
}