#pragma once 

#include <string>
#include <vector>

namespace sb
{
	class File
	{
	public:
		static std::string loadText(const std::string& assetPath);

	protected:
		static std::string assetPathToFilePath(const std::string& assetPath);

		static std::string combinePath(const std::vector<std::string>& parts);

		static std::string getAssetFolderPath();

		static std::string getWorkingDirectory();

	private:
		static const std::string PathSeparator;
	};
}