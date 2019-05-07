#include "Logger.h"

namespace sb
{
	void logMessage(std::stringstream& stream) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, stream.str().c_str());
	}

	void logError(std::stringstream& stream) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, stream.str().c_str());
	}

	void logWarning(std::stringstream& stream) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, stream.str().c_str());
	}
}