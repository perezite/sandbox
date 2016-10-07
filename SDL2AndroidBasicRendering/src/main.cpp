#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "engine.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <string>
using namespace std;

GLuint program;

int loadShader(int type, char * shaderCode)
{
    int length = strlen(shaderCode);

    int shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char **) &shaderCode, NULL);
    glCompileShader(shader);

    return shader;
}

// Initializes the application data
int Init(void) 
{
    // Vertex shaders
    const char* vertexShaderCode = 
        "attribute vec4 vPosition; \n" 
        "void main() {             \n" 
        " gl_Position = vPosition; \n"  
        "}                         \n";

    const char* fragmentShaderCode = 
        "precision mediump float;                 \n"
        "void main() {                            \n"
        " gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);\n"
        "}                                        \n";

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, (char*)vertexShaderCode);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, (char*)fragmentShaderCode);
    GLint linked;

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindAttribLocation(program, 0, "vPosition");
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if(!linked)
    {
       GLint infoLen = 0;
       glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
       if(infoLen > 1)
       {
          char* infoLog = (char *) malloc(sizeof(char) * infoLen);
          glGetProgramInfoLog(program, infoLen, NULL, infoLog);
          free(infoLog);
       }
       glDeleteProgram(program);
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);

    return GL_TRUE;
}

void Display(void)
{
    // Clear the screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);   

    GLfloat vertices[] = {
        -0.5f, 0.0f, -1.0f,
        0.0f, 1.0f, -1.0f,
        0.5f, 0.0f, -1.0f,
    };

    GLfloat colors[] = {
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
    };

    glUseProgram(program);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES,  0, 3);
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

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	int width = mode.w;
	int height = mode.h;

	SDL_Log("Width = %d. Height = %d,\n", width, height);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Init();
    
	window = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
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

        Display();
        
		SDL_GL_SwapWindow(window);
		SDL_Delay(10);
	}

	exit(0);
}
