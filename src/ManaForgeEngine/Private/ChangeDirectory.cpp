#include "EnginePCH.h"
#include "Engine.h"

#include "ChangeDirectory.h"

#include <boost/filesystem.hpp>

#if defined _WIN32 || defined WIN32

void changeDir()
{
	// changes the path so everything we open will be in Resoruce/
	char ownPth[MAX_PATH];

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(nullptr);
	if (hModule == nullptr)
	{
		logger<Fatal>() << "getModuleHandle failed";
	}
	// When passing NULL to GetModuleHandle, it returns handle of exe itself
	GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

	std::string path = ownPth;
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 4; i++){


		do {
			path = path.substr(0, path.size() - 1);
		} while (path[path.size() - 1] != '\\');

	}
	path += "Resource\\";

	boost::filesystem::current_path(path.c_str());


}

#elif defined __linux__

void changeDir()
{
	//TODO write linux code

	return 0;
}

#elif defined __APPLE__
void changeDir()
{
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) == 0)
		printf("executable path is %s\n", path);
	else
		printf("buffer too small; need size %u\n", size);

	std::string pathStr = path;
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 3; i++){

		do {
			path = pathStr.substr(0, pathStr.size() - 1);
		} while (pathStr[pathStr.size() - 1] != '\\');

	}


	boost::filesystem::current_path(path.append("Resources\\"));

}
#endif