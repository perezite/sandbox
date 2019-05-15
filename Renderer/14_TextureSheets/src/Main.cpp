#include "DrawBatch.h"
#include "Window.h"
#include "Input.h"
#include "Sprite.h"
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
	// blockTextures.add("Textures/CyanBlock.png", 0, 0, 121, 121);
	// blockTextures.add("Textures/GreenBlock.png", sb::IntRect(121, 0, 121, 121));
	// blockTextures.add("Textures/PurpleBlock.png", 0, 121);		
	// blockTextures.add("Textures/RedBlock.png", sb::Vector2i(121, 121));
	
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

int main() {
	concept0();
	std::cin.get();

	return 0;
}