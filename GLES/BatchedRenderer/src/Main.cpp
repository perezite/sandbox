#include <iostream>
#include <vector>
#include "Window.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Stopwatch.h"
#include "Error.h"
#include <SDL2/SDL.h>
#include <vector>
using namespace sb;

const unsigned int NumTrianglesHorz = 10; 
const unsigned int NumTrianglesVert = 10; 

void run();
void logPerformance();
std::string getTransparentVertexShaderSource();
std::string getTransparentFragmentShaderSource();

int main(int argc, char* args[])
{
	SDL_Log("Non Indexed Renderer 2: Build %s %s", __DATE__, __TIME__);

	run();
}

void run()
{
	Window window;
	Triangle triangle(Vector2f(-0.5f, -0.3f), Vector2f(0.2f, 0.2f), 0.785398f);
	sb::Rectangle rectangle(Vector2f(0.5f, 0.3f), Vector2f(0.1f, 0.1f), 0);
	sb::Triangle triangle2(Vector2f(0.5f, -0.3f), Vector2f(0.3f, 0.3f), 0.3f);
	sb::Triangle triangle3(Vector2f(-0.3f, 0.3f), Vector2f(0.3f, 0.3f), 0.3f);
	Shader transparentShader; 
	Shader redTintShader;
	transparentShader.loadFromMemory(getTransparentVertexShaderSource(), getTransparentFragmentShaderSource());
	redTintShader.loadFromFile("Shaders/RedTint.vert", "Shaders/RedTint.frag");

	while (window.isOpen()) {
		window.update();
		window.draw(triangle);
		window.draw(rectangle);
		window.draw(triangle2, &transparentShader);
		window.draw(triangle3, &redTintShader);
		window.display();
		logPerformance();
	}
}

std::string getTransparentVertexShaderSource()
{
	return
		"attribute vec2 a_vPosition;										\n"
		"attribute vec4 a_vColor;											\n"
		"varying vec4 v_vColor;												\n"
		"void main()														\n"
		"{																	\n"
		"   gl_Position = vec4(a_vPosition.x, a_vPosition.y, 0 , 1 );		\n"
		"	v_vColor = a_vColor;											\n"
		"}";
}

std::string getTransparentFragmentShaderSource()
{
	return
		"#version 100									\n"
		"precision mediump float;						\n"
		"varying vec4 v_vColor;		 					\n"
		"void main()									\n"
		"{												\n"
		"  gl_FragColor = vec4(v_vColor.xyz, 0.1);		\n"
		"}												\n";
}

void logPerformance()
{
	static Stopwatch stopwatch;
	static unsigned int frames = 0;

	float elapsed = stopwatch.getElapsedSeconds();
	frames++;
	if (elapsed > 1.0f) {
		float fps = frames / elapsed;
		SDL_Log("FPS: %f", fps);
		frames = 0;
		stopwatch.reset();
	}
}
