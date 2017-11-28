#pragma once
#include <string>
#include <filesystem>
#include <chrono>
#include <tchar.h>
#include <Windows.h>
#include <Winuser.h>


struct WindowInfo
{
	std::wstring windowTitle;
	DWORD treadID;
	HWND desriptor;
	bool isActive;
};

class ApplicationInfo
{
public:
	ApplicationInfo();
	virtual ~ApplicationInfo();

	void SetName(const TCHAR * name_of_exe);
	void SetPath(const TCHAR * path_to_exe);
	void SetTitle(const TCHAR * wnd_title);

	const std::wstring& GetName() const;
	const std::wstring& GetPath() const;
	const std::wstring& GetWndTitle() const;

	
	//return true if process has Window descriptor
	bool IsWindowed() const;

	std::vector<WindowInfo> childWindows;
private:
	/* Process */
	std::wstring _windowTitle;
	std::wstring _processName;
	std::wstring _fullPathToExe;

	bool _isWindowed;
	bool _isActive;
	//std::vector<>
	/* exe */
	size_t _sizeInBytes;
	std::chrono::time_point<std::chrono::system_clock> _creationDate;
	std::chrono::time_point<std::chrono::system_clock> _ñhangeDate;


};

