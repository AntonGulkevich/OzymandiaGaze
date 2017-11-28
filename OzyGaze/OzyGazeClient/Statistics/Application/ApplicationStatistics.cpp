#include "ApplicationStatistics.h"


INT ApplicationStatistics::InitializeApplInfo()
{
	// Sync
	std::lock_guard<std::mutex>lg(_applStatGuard);

	// Take a snapshot of all processes in the system.
	const auto hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// If the function fails, it returns INVALID_HANDLE_VALUE.
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcessSnap);
		return GetLastError();
	}
	PROCESSENTRY32 pe32;
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Retrieve information about the first process and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return GetLastError();
	}

	// Walk the snapshot of processes
	do
	{
		// Create a new instance of ProcessInfo using uniqu_ptr
		auto tmpApplInfo = std::make_unique<ProcessInfo>(pe32);

		auto err = tmpApplInfo->Init();
		if (err)
		{
			Process32Next(hProcessSnap, &pe32);
		}
		else
		{
			_applicationInfoVector.emplace_back(std::move(tmpApplInfo));
		}

	} while (Process32Next(hProcessSnap, &pe32));
	// Close hProcessSnap
	CloseHandle(hProcessSnap);
	return 0;
}

ApplicationStatistics::ApplicationStatistics() :_averageCountOfProcesses(50), _maxCountOfProcesses(0), _minCountOfProcesses(), _lastCountOfProcesses()
{
}


ApplicationStatistics::~ApplicationStatistics()
{
}

void ApplicationStatistics::TEST_showProcess()
{
	//for (auto &&pair : _applicationInfoVector)
	//{
	//	if (pair.second->IsWindowed()) {
	//		std::wcout << L"ID:   " << pair.first
	//			<< L"\nName: " << pair.second->GetName()
	//			<< L"\nPath: " << pair.second->GetPath()
	//			<< L"\nWind: " << pair.second->GetWndTitle() << std::endl
	//			<< std::setw(80) << std::setfill(L'=') << "" << std::endl;
	//	}
	//}
}


