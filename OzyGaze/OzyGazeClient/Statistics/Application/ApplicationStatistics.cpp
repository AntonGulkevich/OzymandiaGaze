#include "ApplicationStatistics.h"



INT ApplicationStatistics::InitializeProcessList()
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

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
<<<<<<< HEAD
		// Create a new instance of ProcessInfoTree using uniqu_ptr
		auto tmpApplInfo = std::make_unique<ProcessInfoTree>(pe32);

		const auto err = tmpApplInfo->Init();
		if (err)
=======
		// Create a new instance of ApplicationInfo using uniqu_ptr
		auto tmpApplInfo = std::make_unique<ApplicationInfo>();

		const auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		// If the function fails, the return value is NULL
		if (hProcess == nullptr)
>>>>>>> parent of 6c61b94... cnt
		{
			const auto errC = GetLastError();
			CloseHandle(hProcess);
			//If the specified process is the System Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
			if (pe32.th32ProcessID == 0 && errC == ERROR_INVALID_PARAMETER)
			{
				// Skip System Process
				Process32Next(hProcessSnap, &pe32);
				continue;
			}
			/*
			* If the specified process is the Idle process or one of the CSRSS processes,
			* this function fails and the last error code is ERROR_ACCESS_DENIED
			* because their access restrictions prevent user-level code from opening them.
			*/
			if (errC == ERROR_ACCESS_DENIED)
			{
				// Skip Idle or CSRSS processes
				Process32Next(hProcessSnap, &pe32);
				continue;
			}
			// 
			return errC;
			//LPTSTR messageBuffer = nullptr;
			//const size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			//                                   nullptr, errC, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&messageBuffer), 0, nullptr);
			//const std::wstring message(messageBuffer, size);
			//tmpApplInfo->SetPath(message.c_str());
			////Free the buffer.
			//LocalFree(messageBuffer);
		}

		TCHAR szModName[MAX_PATH];
		// If the function fails, the return value is zero.
		if (GetModuleFileNameEx(hProcess, nullptr, szModName, sizeof(szModName) / sizeof(TCHAR)) == 0)
		{
<<<<<<< HEAD
			_processInfoTree.emplace_back(std::move(tmpApplInfo));
=======
			CloseHandle(hProcess);
			CloseHandle(hProcessSnap);
			return GetLastError();
>>>>>>> parent of 6c61b94... cnt
		}
		CloseHandle(hProcess);
		tmpApplInfo->SetPath(szModName);
		tmpApplInfo->SetName(pe32.szExeFile);
		// Add Process to map
		_applicationInfoMap.insert(std::make_pair(pe32.th32ProcessID, std::move(tmpApplInfo)));


	} while (Process32Next(hProcessSnap, &pe32));
	// Close hProcessSnap
	CloseHandle(hProcessSnap);

	// Enum Windows 
	EnumWindows(EnumerateWindowsHandlers, reinterpret_cast<LPARAM>(&_applicationInfoMap));

	// Get file information

	// Get file info size
	for (auto &&pair : _applicationInfoMap)
	{
		//If the function fails, the return value is zero.
		const auto retSize = GetFileVersionInfoSize(pair.second->GetPath().c_str(), nullptr);
		if (!retSize)
		{
			return GetLastError();
		}
		auto pBlock = static_cast<PLONG>(malloc(retSize));
		//If the function fails, the return value is zero.
		if (!GetFileVersionInfo(pair.second->GetPath().c_str(), NULL, retSize, pBlock))
		{
			free(pBlock);
			return GetLastError();
		}

		const TCHAR *paramNames[] = {
			_T("FileDescription"),
			_T("CompanyName"),
			_T("FileVersion"),
			_T("InternalName"),
			_T("LegalCopyright"),
			_T("LegalTradeMarks"),
			_T("OriginalFilename"),
			_T("ProductName"),
			_T("ProductVersion"),
			_T("Comments"),
			_T("Author")
		};

		struct LANGANDCODEPAGE { 
			WORD wLanguage;
			WORD wCodePage;
		} *pLangCodePage;

		UINT cpSz;

		if (!VerQueryValue(pBlock, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&pLangCodePage), &cpSz))
		{
			free(pBlock);
			return GetLastError();
		}


		TCHAR paramNameBuf[256]; // здесь формируем имя параметра
		TCHAR *paramValue;       // здесь будет указатель на значение параметра, который нам вернет функция VerQueryValue
		UINT paramSz;            // здесь будет длина значения параметра, который нам вернет функция VerQueryValue

		for (int cpIdx = 0; cpIdx < (int)(cpSz / sizeof(struct LANGANDCODEPAGE)); cpIdx++)
		{
			// перебираем имена параметров
			for (int paramIdx = 0; paramIdx < sizeof(paramNames) / sizeof(char*); paramIdx++)
			{
				// формируем имя параметра ( \StringFileInfo\кодовая_страница\имя_параметра )
				_stprintf(paramNameBuf, _T("\\StringFileInfo\\%04x%04x\\%s"),
					pLangCodePage[cpIdx].wLanguage,  // ну, или можно сделать фильтр для
					pLangCodePage[cpIdx].wCodePage,  // какой-то отдельной кодовой страницы
					paramNames[paramIdx]);

				if (VerQueryValue(pBlock, paramNameBuf, (LPVOID*)&paramValue, &paramSz))
					std::wcout << L"\t\t" << paramNames[paramIdx] << L":\t\t" << paramValue << std::endl;
				else
					std::wcout << L"\t\t" << paramNames[paramIdx] << L"\t\tHет информации " << std::endl;
			}
		}
		free(pBlock);
	}
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
<<<<<<< HEAD
	//for (auto &&pair : _processInfoTree)
	//{
	//	if (pair.second->IsWindowed()) {
	//		std::wcout << L"ID:   " << pair.first
	//			<< L"\nName: " << pair.second->GetName()
	//			<< L"\nPath: " << pair.second->GetPath()
	//			<< L"\nWind: " << pair.second->GetWndTitle() << std::endl
	//			<< std::setw(80) << std::setfill(L'=') << "" << std::endl;
	//	}
	//}
=======
	for (auto &&pair : _applicationInfoMap)
	{
		if (pair.second->IsWindowed()) {
			std::wcout << L"ID:   " << pair.first
				<< L"\nName: " << pair.second->GetName()
				<< L"\nPath: " << pair.second->GetPath()
				<< L"\nWind: " << pair.second->GetWndTitle() << std::endl
				<< std::setw(80) << std::setfill(L'=') << "" << std::endl;
		}
	}
>>>>>>> parent of 6c61b94... cnt
}

BOOL ApplicationStatistics::EnumerateWindowsHandlers(const HWND hwnd, const LPARAM l_param)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	auto &applMap = *reinterpret_cast<std::map<DWORD, std::unique_ptr<ApplicationInfo>>*>(l_param);
	auto applInfo = applMap.find(lpdwProcessId);
	if (applInfo == applMap.end())
		return TRUE;
	//If the window has no text, the return value is zero.
	const auto titleLength = GetWindowTextLength(hwnd);
	if (titleLength == 0)
	{
		applInfo->second->SetTitle(TEXT("Unknown"));
		return TRUE;
	}
	const auto strBuffer = new TCHAR[titleLength+1];

	//If the window has no text, the return value is zero.
	const auto retLength = GetWindowText(hwnd, strBuffer, titleLength+1);
	if (retLength == 0)
	{
		applInfo->second->SetTitle(TEXT("Unknown"));
		delete[]strBuffer;
		return TRUE;
	}
	applInfo->second->SetTitle(strBuffer);
	delete[]strBuffer;
	return TRUE;
}
