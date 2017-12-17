#pragma once

#include <Windows.h>
#include <psapi.h> // Windows process API
#include <vector>
#include <mutex>
#include <tlhelp32.h> // CreateToolhelp32Snapshot
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>

#include "ProcessInfo.h"
#include "../../../ErrorOzy/RtOzy.h"
#include "../../Misc/RtCrc32.h" /// crc 32

#pragma  comment(lib, "Version.lib") // GetFileVersionInfoSize


class ApplicationStatistics
{
public:

	ApplicationStatistics();
	virtual ~ApplicationStatistics();

	/**
	 * \brief Fill application info map 
	 * \return zero on succsess and error code on failure 
	 */
	INT InitializeApplInfo();

	// Update foregrounf info
	void GetForegroundWndInfo();

	// Print Foreground windows info into console
	void PrintForegroundInfo(uint32_t flags);

#pragma region TEST
	void TEST_showProcess();
#pragma endregion
	
private:
#pragma region Sync
	std::mutex _applStatGuard;
#pragma endregion

	//
	std::vector<std::unique_ptr<ProcessInfo>> _applicationInfoVector;
	// Average count of precesses
	DWORD _averageCountOfProcesses;
	// Max count of processes
	DWORD _maxCountOfProcesses;
	// Min count of processes
	DWORD _minCountOfProcesses;
	// The last count of processes
	DWORD _lastCountOfProcesses;

	// Verison info cache with CRC32 as Key
	std::map<DWORD, std::shared_ptr<DWORD>> _versionInfoCache;

	// Foreground wnd info map
};

