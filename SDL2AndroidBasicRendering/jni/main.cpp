#include <jni.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

static const int32_t POSITION_PARAMETER_INDEX = 0;
static const int32_t COLOR_PARAMETER_INDEX = 1;

struct engine
{
	struct android_app* app;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t programObject;
	float width;
	float height;
	float touchX;
	float touchY;
	bool touchIsDown;
};

GLuint LoadShader(const char *shaderSrc, GLenum type)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);
	if(shader != 0)
	{
		// Load the shader source
		glShaderSource(shader, 1, &shaderSrc, NULL);

		// Compile the shader
		glCompileShader(shader);
		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if(infoLen > 1)
			{
				char* infoLog = new char[infoLen];
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				LOGW("Error compiling shader:\n%s\n", infoLog);
				delete[] infoLog;
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

static int initEGL(struct engine* engine)
{
	// initialize OpenGL ES and EGL

	/*
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] =
	{
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, NULL, NULL);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	EGLint contextAttribs[] =
	{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
	};
	context = eglCreateContext(display, config, NULL, contextAttribs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->touchX = 0.0f;
	engine->touchY = 0.0f;
	engine->touchIsDown = false;  
}

static int init(struct engine* engine) {
	// Initialize GL state.
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	char vShaderStr[] =
			"attribute vec4 a_vPosition;   \n"
			"attribute vec4 a_vColor;	   \n"
			"varying vec4 v_vColor;		   \n"
			"void main()                   \n"
			"{                             \n"
			"   gl_Position = a_vPosition; \n"
			"	v_vColor = a_vColor;       \n"
			"}                             \n";

	char fShaderStr[] =
			"precision mediump float;                   \n"
			"varying vec4 v_vColor;		 				\n"
			"void main()                                \n"
			"{                                          \n"
			"  gl_FragColor = v_vColor;					\n"
			"}                                          \n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = LoadShader(vShaderStr, GL_VERTEX_SHADER);
	fragmentShader = LoadShader(fShaderStr, GL_FRAGMENT_SHADER);

	// Create the program object
	engine->programObject = glCreateProgram();
	if(engine->programObject == 0)
	{
		return -1;
	}

	// Attach shaders to program
	glAttachShader(engine->programObject, vertexShader);
	glAttachShader(engine->programObject, fragmentShader);

	// Bind a_vPosition to attribute 0 and a_vColor to 1
	glBindAttribLocation(engine->programObject, POSITION_PARAMETER_INDEX, "a_vPosition");
	glBindAttribLocation(engine->programObject, COLOR_PARAMETER_INDEX, "a_vColor");

	// Link the program
	glLinkProgram(engine->programObject);

	// Check the link status
	glGetProgramiv(engine->programObject, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(engine->programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = new char[infoLen];
			glGetProgramInfoLog(engine->programObject, infoLen, NULL, infoLog);
			LOGW("Error linking program:\n%s\n", infoLog);

			delete[] infoLog;
		}

		glDeleteProgram(engine->programObject);
		return -1;
	}

	return 0;
}

static void display(struct engine* engine)
{ 
    static const int32_t PositionNumElements = 3;
    static const int32_t ColorNumElements = 4;
    static const int32_t VertexSize = sizeof(GLfloat) * (PositionNumElements + ColorNumElements);

	glViewport(0, 0, static_cast<int32_t>(engine->width), static_cast<int32_t>(engine->height));

	// Just fill the screen with a color.
	glClearColor(0.95f, 0.95f, 0.95f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(engine->programObject);

	glEnableVertexAttribArray(POSITION_PARAMETER_INDEX);
	glEnableVertexAttribArray(COLOR_PARAMETER_INDEX);

	const float z = 0.0f; 
	float left = -0.1f;
	float right = 0.1f;
	float top = 0.1f;
	float bottom = -0.1f;
	const float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat triangle[] = { 0.0f, top, z,
        color[0], color[1], color[2], color[3],
        left, bottom, z,
        color[0], color[1], color[2], color[3],
        right, bottom, z,
        color[0], color[1], color[2], color[3] };

	glVertexAttribPointer(POSITION_PARAMETER_INDEX, PositionNumElements, GL_FLOAT, GL_FALSE, VertexSize, triangle);
	glVertexAttribPointer(COLOR_PARAMETER_INDEX, ColorNumElements, GL_FLOAT, GL_FALSE, VertexSize, &triangle[3]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
    
	glDisableVertexAttribArray(POSITION_PARAMETER_INDEX);
	glDisableVertexAttribArray(COLOR_PARAMETER_INDEX);
}

static void close(struct engine* engine)
{
	if (engine->display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

static int32_t input(struct android_app* app, AInputEvent* event)
{
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		int32_t ret = 0;

		int32_t action = AMotionEvent_getAction(event);
		if (action == AMOTION_EVENT_ACTION_DOWN)
		{
			engine->touchIsDown = true;
			ret = 1;
		}
		else if (action == AMOTION_EVENT_ACTION_UP)
		{
			engine->touchIsDown = false;
			ret = 1;
		}

		if (ret)
		{
			engine->touchX = static_cast<float>(AMotionEvent_getRawX(event, 0)) / engine->width;
			engine->touchY = static_cast<float>(AMotionEvent_getRawY(event, 0)) / engine->height;
		}
		return ret;
	}
	return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
	struct engine* engine = static_cast<struct engine*>(app->userData);
	switch (cmd)
	{
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL)
		{
            initEGL(engine);
			init(engine);
			display(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		close(engine);
		break;
	}
}

void android_main(struct android_app* state)
{
	struct engine engine;

	// Make sure glue isn't stripped.
	app_dummy();

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = input;
	engine.app = state;

	// loop waiting for stuff to do.
	while (1) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident=ALooper_pollAll(0, NULL, &events, reinterpret_cast<void**>(&source))) >= 0)
		{
			// Process this event.
			if (source != NULL)
			{
				source->process(state, source);
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0)
			{
				close(&engine);
				return;
			}
		}
        
		display(&engine);
        eglSwapBuffers(engine.display, engine.surface);
	}
}
