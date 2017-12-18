#include "ApplicationStatistics.h"


INT ApplicationStatistics::InitializeApplInfo()
{
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
		CloseHandle(hProcessSnap); // clean the snapshot object
		return GetLastError();
	}

	// Walk the snapshot of processes
	do
	{
		// Create a new instance of ProcessInfo using uniqu_ptr
		auto tmpApplInfo = std::make_unique<ProcessInfo>(pe32);

		auto err = tmpApplInfo->InitWndThree();
		if (err)
			Process32Next(hProcessSnap, &pe32);
	} while (Process32Next(hProcessSnap, &pe32));
	// Close hProcessSnap
	CloseHandle(hProcessSnap);

	return 0;
}
