#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string>

#include "SDL.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/log.h>

#define _USE_MATH_DEFINES

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

using namespace std;

static const int32_t POSITION_PARAMETER_INDEX = 0;
static const int32_t COLOR_PARAMETER_INDEX = 1;

int32_t shaderProgram;
float width;
float height;

static void display()
{     
	// Just fill the screen with a color.
    float rr = (float)(rand()%255)/255.0f;
    float rg = (float)(rand()%255)/255.0f;
    float rb = (float)(rand()%255)/255.0f;
    glClearColor(rr, rg, rb, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


int main(int argc, char *argv[])
{    
	SDL_Window* window = 0;
	SDL_GLContext gl = 0;

	if (0 != SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
    
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
   
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	width = mode.w;
	height = mode.h;

	window = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);

	if (window == 0)
	{
		SDL_Log("Failed to create window.");
		SDL_Quit();
		return 1;
	}

	gl = SDL_GL_CreateContext(window);
    
	Uint8 done = 0;
	SDL_Event event;
	int count = 0;
	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN)
			{
				done = 1;
			}
		}

		SDL_Log("%d\n", count++);

        display();
             
		SDL_GL_SwapWindow(window);
	}

	exit(0);
}
