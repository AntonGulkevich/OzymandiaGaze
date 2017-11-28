#include "ApplicationStatistics.h"


INT ApplicationStatistics::InitializeApplInfo()
{
	_applicationInfoVector.clear();
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
			Process32Next(hProcessSnap, &pe32);
		else
			_applicationInfoVector.emplace_back(std::move(tmpApplInfo));

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
	const auto utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	static std::wofstream global_fs;

	global_fs.open("test.txt", std::wofstream::out);
	global_fs.imbue(utf8_locale);


	for (auto&& process_info : _applicationInfoVector)
	{
		global_fs << std::setw(20) << std::left << L"Exe name:" << process_info->GetExeName() << std::endl;
		global_fs << std::setw(20) << std::left << L"Full path to exe:" << process_info->GetExePath() << std::endl;
		global_fs << std::setw(20) << std::left << L"Process ID:" << process_info->GetProcessId() << std::endl;
		global_fs << process_info->_windowsInfoTree;
		global_fs << L"================================================================================" << std::endl;
	}
	
	
	//global_fs << std::setw(80) << std::setfill(L'=') <<L"=" <<std::endl;
	//global_fs << std::setfill(L' ') << std::setw(0);

	global_fs.close();
	
}


