#pragma once

#if defined(__INTELLISENSE__) && defined(__ANDROID__) 
#undef __MMX__
#undef __SSE2__
#undef __SSE__
#endif

#include "Logger.h"
#include <SDL2/SDL.h>

namespace sb {
	void glCheckError(const char* command, const char* file, unsigned int line);
}

#define SDL_CHECK(condition) do { if (!!!(condition)) SB_ERROR(SDL_GetError()); } while (false)
