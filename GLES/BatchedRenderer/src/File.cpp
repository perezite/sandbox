#include "File.h"

#include "Error.h"
#ifdef WIN32
	#include <Windows.h>
#endif
#include <SDL2/SDL.h>

namespace sb
{
	const std::string File::PathSeparator = "\\";

	std::string File::loadText(const std::string& assetPath)
	{
		std::string filePath = assetPathToFilePath(assetPath);

		// setup
		SDL_RWops* reader = SDL_RWFromFile(filePath.c_str(), "r");
		Sint64 fileSize = SDL_RWsize(reader);
		char* data = (char*)malloc((size_t)fileSize + 1);

		Sint64 totalSize = 0;
		Sint64 lastSize = 0;

		// read
		do
		{
			lastSize = SDL_RWread(reader, &data[totalSize], sizeof(char), (size_t)(fileSize - totalSize));
			totalSize += lastSize;
		} while (totalSize < fileSize && lastSize != 0);
		data[totalSize] = '\0';
		SDL_RWclose(reader);

		// check
		Error().dieIf(totalSize != fileSize) << "Could not load file '" << assetPath << "'";

		return data;
	}

	std::string File::assetPathToFilePath(const std::string& assetPath)
	{
		return combinePath({ getAssetFolderPath(), assetPath });

	}

	std::string File::combinePath(const std::vector<std::string>& parts)
	{
		std::string result;
		for (unsigned int i = 0; i < parts.size() - 1; i++) {
			if (parts[i].size() > 0) {
				result += parts[i];
				result += PathSeparator;
			}
		}

		result += parts[parts.size() - 1];

		return result;
	}

	std::string File::getAssetFolderPath()
	{
		#ifdef WIN32
			return combinePath({ getWorkingDirectory(), "..", "Assets" });
		#endif

		#ifdef __ANDROID__
			return "";
		#endif
	}

	std::string File::getWorkingDirectory()
	{
		#ifdef WIN32
		wchar_t filePath[MAX_PATH];

		// get executable file path
		HMODULE hModule = GetModuleHandle(NULL);
		GetModuleFileName(hModule, filePath, (sizeof(filePath)));

		// get file path as string
		std::wstring filePathAsWideString(filePath);
		std::string filePathAsString(filePathAsWideString.begin(), filePathAsWideString.end());

		// remove trailing file name
		size_t lastPathSeparatorPosition = filePathAsString.rfind(PathSeparator);
		std::string directoryPathAsString = filePathAsString.substr(0, lastPathSeparatorPosition);

		return directoryPathAsString;
		#endif

		#ifdef __ANDROID__
			Error().die() << "This functionality is not supported on Android";
		#endif
	}
}