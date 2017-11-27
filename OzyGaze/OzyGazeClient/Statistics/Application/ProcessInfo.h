#pragma once
#include <string>
#include <filesystem>
#include <chrono>
#include <tchar.h>
#include <Windows.h>
#include <Winuser.h>
#include <tlhelp32.h>


struct WindowInfo
{
	std::wstring windowTitle;
	DWORD treadID;
	HWND desriptor;
	bool isActive;
};

struct EnumWindowsCallbackArgs {
	explicit EnumWindowsCallbackArgs(DWORD PID):pid (PID){}
	DWORD pid;
	std::vector<WindowInfo> handles;
};

class ProcessInfo
{
public:
	explicit ProcessInfo(const PROCESSENTRY32 &prEntry);
	virtual ~ProcessInfo();

	// Return 0 on sucess, error code on failure
	int UpdateInfo();
	// Return 0 on sucess, error code on failure
	int Init();
	//return true if process has Window descriptor
	bool IsWindowed() const;

	const std::wstring& GetName() const;
	const std::wstring& GetPath() const;
	const std::wstring& GetWndTitle() const;

private:

	static BOOL CALLBACK EnumerateWindowsHandlers(HWND hwnd, LPARAM lParam);

	EnumWindowsCallbackArgs _windows;
	std::vector<EnumWindowsCallbackArgs> _childWindows;
	std::shared_ptr<PROCESSENTRY32> _prEntry;
	/* Process */
	std::wstring _fullPathToExe;
	bool _hasWindow;
	bool _isWindowActive;
	std::wstring _windowTitle;
	//std::chrono::time_point<std::chrono::system_clock> _creationDate;
	//std::chrono::time_point<std::chrono::system_clock> _ñhangeDate;


};

