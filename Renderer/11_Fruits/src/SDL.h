#pragma once
#include "Logger.h"
#include <SDL2/SDL.h>

namespace sb {
	void glCheckError(const char* command, const char* file, unsigned int line);
}

#define SDL_CHECK(condition) do { if (!!!(condition)) SB_ERROR2(SDL_GetError()); } while (false)
