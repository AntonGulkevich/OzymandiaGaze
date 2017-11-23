#include "ApplicationInfo.h"



ApplicationInfo::ApplicationInfo(): wndTree(nullptr), _isWindowed(false), _isActive(false), _sizeInBytes(0)
{
}


ApplicationInfo::~ApplicationInfo()
{
}

void ApplicationInfo::SetName(const TCHAR* name_of_exe)
{
	_processName = std::wstring(name_of_exe);
}

void ApplicationInfo::SetPath(const TCHAR* path_to_exe)
{
	_fullPathToExe = std::wstring(path_to_exe);
}

void ApplicationInfo::SetTitle(const TCHAR* wnd_title)
{
	_isWindowed = true;
	_windowTitle = std::wstring(wnd_title);
}

const std::wstring& ApplicationInfo::GetName() const
{
	return _processName;
}

const std::wstring& ApplicationInfo::GetPath() const
{
	return _fullPathToExe;
}

const std::wstring& ApplicationInfo::GetWndTitle() const
{
	return _windowTitle;
}

bool ApplicationInfo::IsWindowed() const
{
	return _isWindowed;
}
