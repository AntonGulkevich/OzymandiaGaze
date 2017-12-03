#pragma once
#include <string>
#include <filesystem>
#include <chrono>
#include <tchar.h>
#include <Windows.h>
#include <Winuser.h>
#include <tlhelp32.h>
#include <fstream>
#include <iomanip>
#include <psapi.h>
#include <iostream>

struct WindowInfo
{
	HWND parentHwnd = nullptr;
	HWND hwnd = nullptr;
	bool isActive = false;
	std::wstring window_title = std::wstring(TEXT("Unknown"));
	std::vector<WindowInfo*> childWindowsVector;
	UINT zOrder = 0;
	UINT maxZorder = 0;
	friend std::wostream& operator<<(std::wostream& os, const WindowInfo& window_info)
	{
		os << std::right << std::setw(window_info.zOrder * 2) << L"|" << L" WND NAME: " << window_info.window_title.c_str() << std::endl;
		for (auto child_wnd_info : window_info.childWindowsVector)
		{
			os << *child_wnd_info;
		}
		return os;
	}
};

struct WinTreeInfo
{
	explicit WinTreeInfo(DWORD pid) : processId(pid), _maxDeep(0)
	{
	}

	// parent PID
	DWORD processId;
	// Tree of windows
	std::vector<WindowInfo*> _windowInfoTree;
	// Max deep of child windows
	DWORD _maxDeep;

	friend std::wostream& operator<<(std::wostream& os, const WinTreeInfo& win_tree_info)
	{
		for (auto window_info : win_tree_info._windowInfoTree)
			os << *window_info;
		return os;
	}
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

	auto GetExeName()
	{
		return _prEntry->szExeFile;
	}

	auto GetExePath()
	{
		return _fullPathToExe;
	}
	DWORD GetProcessId() const;

	WinTreeInfo _windowsInfoTree;

private:
	// full path to exe file 
	std::wstring _fullPathToExe;
	// process info struct
	std::shared_ptr<PROCESSENTRY32> _prEntry;
	// Enumerate top level windows assiciated with this process id
	static BOOL CALLBACK EnumerateWindowsHandlers(HWND hwnd, LPARAM lParam);
	// Enumerate child windows created by top level windows
	static BOOL CALLBACK EnumerateChildWindowsHandlers(HWND hwnd, LPARAM lParam);

	//std::chrono::time_point<std::chrono::system_clock> _creationDate;
	//std::chrono::time_point<std::chrono::system_clock> _ñhangeDate;


};

