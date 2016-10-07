#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "engine.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <string>
using namespace std;

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "void main()"
    "{"
    "    gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "out vec4 outColor;"
    "void main()"
    "{"
    "    outColor = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";

// Initializes the application data
void Init(void) 
{
    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Display()
{
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Cleanup()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
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
    
	window = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    Init();
    
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
    
    Cleanup();

	exit(0);
}
