#pragma once

#include <Windows.h>
#include <psapi.h> // Windows process API
#include <vector>
#include <mutex>
#include <tlhelp32.h> // CreateToolhelp32Snapshot
#include <iostream>
#include <iomanip>
#include <map>


#include "ProcessInfo.h"
#include "../../../ErrorOzy/RtOzy.h"

#pragma  comment(lib, "Version.lib") // GetFileVersionInfoSize

class ApplicationStatistics
{
public:
	/**
	 * \brief Fill application info map 
	 * \return zero on succsess and error code on failure 
	 */
	INT InitializeApplInfo();

	ApplicationStatistics();
	virtual ~ApplicationStatistics();
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
	std::map<DWORD, std::shared_ptr<DWORD>> versionInfoCache;
};

