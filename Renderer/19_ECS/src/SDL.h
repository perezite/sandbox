// get rid of bogus intellisense errors in vs17 when including sdl2
#if defined(__INTELLISENSE__) && defined(__ANDROID__) 
	#undef __MMX__
	#undef __SSE2__
	#undef __SSE__
#endif

#pragma once

#include "Logger.h"
#include <SDL2/SDL.h>

namespace sb {
	void glCheckError(const char* command, const char* file, unsigned int line);
}

#define SDL_CHECK(condition) do { if (!!!(condition)) SB_ERROR(SDL_GetError()); } while (false)
