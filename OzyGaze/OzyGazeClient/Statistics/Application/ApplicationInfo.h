#pragma once
#include <string>
#include <filesystem>
#include  <chrono>

class ApplicationInfo
{
public:
	ApplicationInfo();
	virtual ~ApplicationInfo();

private:
	/* Process */
	std::wstring _windowTitle;
	std::wstring _processName;
	std::experimental::filesystem::path _fullPathToExe;
	std::vector<>
	/* exe */
	size_t _sizeInBytes;
	std::chrono::time_point<std::chrono::system_clock> _creationDate;
	std::chrono::time_point<std::chrono::system_clock> _ñhangeDate;


};

