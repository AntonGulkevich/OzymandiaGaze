#pragma once

#include <Windows.h>
#include <psapi.h> // Windows process API
#include <map>
#include <mutex>
#include <tlhelp32.h> // CreateToolhelp32Snapshot
#include <iostream>
#include <iomanip>

#include "ApplicationInfo.h"
#include "../../../ErrorOzy/RtOzy.h"

#pragma  comment(lib, "Version.lib") // GetFileVersionInfoSize

class ApplicationStatistics
{
public:
	/**
	 * \brief Fill map of application info using current running processes
	 * \return zero on succsess and error code on failure 
	 */
	INT InitializeProcessList();

	ApplicationStatistics();
	virtual ~ApplicationStatistics();
#pragma region TEST
	void TEST_showProcess();
#pragma endregion

private:
#pragma region Sync
	std::mutex _applStatGuard;
#pragma endregion

	static BOOL CALLBACK EnumerateWindowsHandlers(HWND hwnd, LPARAM lParam);

	// Map of pairs (PID, ApplicationInfo)
	std::map<DWORD, std::unique_ptr<ApplicationInfo>> _applicationInfoMap;
	// Average count of precesses
	DWORD _averageCountOfProcesses;
	// Max count of processes
	DWORD _maxCountOfProcesses;
	// Min count of processes
	DWORD _minCountOfProcesses;
	// The last count of processes
	DWORD _lastCountOfProcesses;

};

