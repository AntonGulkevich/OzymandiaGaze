#include "ApplicationStatistics.h"


INT ApplicationStatistics::InitializeProcessList()
{
	// Sync
	std::lock_guard<std::mutex>lg(applStatGuard);

	// Take a snapshot of all processes in the system.
	auto hProcessesSnapshot = Createtool 
	// Create an array of DWORD with size of _AverageCountOfProcesses
	//auto pidRawArray = new DWORD[_AverageCountOfProcesses];
	//auto currentCountOfProcesses = _AverageCountOfProcesses;
	//// Get processes
	//auto err = EnumProcesses(pidRawArray, currentCountOfProcesses, &_LastCountOfProcesses);
	//// On error return error code and exit
	//if (err) {
	//	delete[]pidRawArray;
	//	return GetLastError();
	//}
	//// Calculate count of real processes
	//_LastCountOfProcesses = _LastCountOfProcesses / sizeof(DWORD) * 1.1;
	//// If _AverageCountOfProcesses in less than _LastCountOfProcesses
	//if (_LastCountOfProcesses > _AverageCountOfProcesses)
	//{
	//	// reallocate space for new count
	//	delete[]pidRawArray;
	//	pidRawArray = new DWORD[_LastCountOfProcesses];
	//	// Get processes
	//	err = EnumProcesses(pidRawArray, _LastCountOfProcesses, &currentCountOfProcesses);
	//	// On error return error code and exit
	//	if (err)
	//	{
	//		delete[]pidRawArray;
	//		return GetLastError();
	//	}
	//	// Store count of processes
	//	_LastCountOfProcesses = currentCountOfProcesses;
	//	// Fill map
	//	for (auto i = 0; i < _LastCountOfProcesses; ++i)
	//	{
	//		// Create a new instance of ApplicationInfo using uniqu_ptr
	//		auto tmpApplInfo = std::make_unique<ApplicationInfo>();
	//		// Get a handle to the process.
	//		auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |	PROCESS_VM_READ,FALSE, pidRawArray[i]);
	//		// On error return error code and exit
	//		if (hProcess == nullptr)
	//		{
	//			delete[]pidRawArray;
	//			return GetLastError();
	//		}
	//		// Create single handle for modules in process
	//		HMODULE hMod;
	//		// Count of modules in process
	//		DWORD hModsNeeded;
	//		// If the function succeeds, the return value is nonzero.
	//		err = EnumProcessModules(hProcess, &hMod, sizeof(hMod), &hModsNeeded);
	//		// If hModsNeeded is greater than sizeof(hMod), increase the size of the array and call EnumProcessModules again.
	//		if (hModsNeeded > sizeof(hMod))
	//		{
	//			
	//		}



	//		// GetModuleBaseName
	//		// EnumProcessModules
	//		//	if (NULL != hProcess)
	//		//	{
	//		//		HMODULE hMod;
	//		//		DWORD cbNeeded;
	//		//
	//		//		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
	//		//			&cbNeeded))
	//		//		{
	//		//			GetModuleBaseName(hProcess, hMod, szProcessName,
	//		//				sizeof(szProcessName) / sizeof(TCHAR));
	//		//		}
	//		//	}
	//		//
	//		//	// Print the process name and identifier.
	//		//
	//		//	_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	//		//	// Release the handle to the process.
	//		CloseHandle(hProcess);
	//	}
	}

	delete[]pidRawArray;
}

ApplicationStatistics::ApplicationStatistics():_AverageCountOfProcesses(50), _MaxCountOfProcesses(0), _MinCountOfProcesses(), _LastCountOfProcesses()
{
}


ApplicationStatistics::~ApplicationStatistics()
{
}

BOOL ApplicationStatistics::InitProcessCount()
{
	
}
