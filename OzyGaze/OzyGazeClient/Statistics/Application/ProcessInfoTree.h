#pragma once
#include <string>
#include <filesystem>
#include <chrono>
#include <tchar.h>
#include <Windows.h>
#include <Winuser.h>
#include <tlhelp32.h>

struct WindowsInfo
{

	WindowsInfo()
		: _windowTitle(TEXT("UNKNOWN")),
		_desriptor(nullptr),
		_isActive(false)
	{
	}

	std::wstring _windowTitle;
	HWND _desriptor;
	bool _isActive;
	std::vector<WindowsInfo*> _childWindows;
};


typedef class ProcessInfo
{
public:
	explicit ProcessInfo(const DWORD pid) : _pid(pid) {}

	//void SetWindowsInfo(std::unique_ptr<WindowsInfo> windows_info) { _windowsInfo = std::move(windows_info); }

	auto GetPid() { return _pid; }

	void AddWindowInfo(WindowsInfo* wnd_info) { _topLevelWindows.emplace_back(wnd_info); }
	void AddChildWindowInfo(WindowsInfo* wnd_info) { _childWindows.emplace_back(wnd_info); }

private:
	DWORD _pid;
	std::vector<WindowsInfo*> _topLevelWindows;

} *LPPROCESSINFO;

class ProcessInfoTree
{
public:
	explicit ProcessInfoTree(std::unique_ptr<PROCESSENTRY32> pr_entry);
	virtual ~ProcessInfoTree();

	// Return 0 on sucess, error code on failure
	int Update();
	// Return 0 on sucess, error code on failure
	int Init();

	// Getters
	const std::wstring & GetPathToExe() const { return _fullPathToExe; }

private:

	static BOOL CALLBACK EnumerateWindowsHandlers(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK EnumerateChildWindowsHandlers(HWND hwnd, LPARAM lParam);

	std::shared_ptr<PROCESSENTRY32> _prEntry;
	std::wstring _fullPathToExe;

	LPPROCESSINFO _windows;
};

