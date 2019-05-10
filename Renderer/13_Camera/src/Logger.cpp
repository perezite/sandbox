#include "Logger.h"

namespace sb
{
	void logMessage(std::stringstream& stream) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", stream.str().c_str());
	}

	void logError(std::stringstream& stream) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", stream.str().c_str());
		handleError(stream);
	}

	void logWarning(std::stringstream& stream) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", stream.str().c_str());
	}

	void handleError(std::stringstream& stream) {
		#if defined(WIN32) && defined(_DEBUG) 
				__debugbreak();
		#endif	

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A problem occured", stream.str().c_str(), NULL);
		exit(0);
	}
}