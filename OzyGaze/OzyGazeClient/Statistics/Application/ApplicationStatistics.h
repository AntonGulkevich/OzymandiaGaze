#pragma once

#include <Windows.h>
#include <psapi.h> // Windows process API
#include <map>
#include <mutex>
#include <tlhe>

#include "ApplicationInfo.h"
#include "../../../ErrorOzy/RtOzy.h"

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
private:
#pragma region Sync
	std::mutex applStatGuard;
#pragma endregion

	// Map of pairs (PID, ApplicationInfo)
	std::map<DWORD, std::unique_ptr<ApplicationInfo>> _applicationInfoMap;
	// Average count of precesses
	DWORD _AverageCountOfProcesses;
	// Max count of processes
	DWORD _MaxCountOfProcesses;
	// Min count of processes
	DWORD _MinCountOfProcesses;
	// The last count of processes
	DWORD _LastCountOfProcesses;

};

