#include "Window.h"
#include "DrawBatch.h"
#include "Input.h"
#include "Quad.h"
#include "Shader.h"
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <string>

#define SHADER_CODE(s) #s

void demo0() {
	std::string test = SHADER_CODE(
		line1	\n
		line2	\n
		line	\n
	);
	std::cout << test << std::endl;
	std::cin.get();
}


void demo1() {
	std::string vertexShader = SHADER_CODE(
		attribute vec2 position;																\n
		attribute vec4 color;																	\n
		uniform mat3 transform;																	\n
		varying vec4 v_color;																	\n
		vec3 transformedPosition;																\n
		void main()																				\n
		{																						\n
		   transformedPosition = transform * vec3(position.x, position.y, 1);					\n
		   gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0, 1 );				\n
			v_color = color;																	\n
		}																						\n
	);

	std::string fragmentShader = SHADER_CODE(
		#version 100									\n
		precision mediump float;						\n
		varying vec4 v_color;		 					\n
		void main()										\n
		{												\n
			gl_FragColor = v_color;						\n
		}												\n
	);

	sb::Window window;
	sb::Shader shader;
	shader.loadFromMemory(vertexShader, fragmentShader);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		window.clear();
		window.display();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Texture Renderer: Build %s %s", __DATE__, __TIME__);

	demo1();
}
