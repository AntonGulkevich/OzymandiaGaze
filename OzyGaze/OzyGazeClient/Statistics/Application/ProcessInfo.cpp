#include "ProcessInfo.h"
#include <psapi.h>
#include <iostream>

ProcessInfo::ProcessInfo(const PROCESSENTRY32 &prEntry) : _windows(prEntry.th32ProcessID), _prEntry(std::make_shared<PROCESSENTRY32>(prEntry)), _hasWindow(false), _isWindowActive(false)
{
}

ProcessInfo::~ProcessInfo()
{
}

int ProcessInfo::UpdateInfo()
{
	return 0;
}

const std::wstring& ProcessInfo::GetName() const
{
	return _prEntry->szExeFile;
}

const std::wstring& ProcessInfo::GetPath() const
{
	return _fullPathToExe;
}

const std::wstring& ProcessInfo::GetWndTitle() const
{
	return _windowTitle;
}

bool ProcessInfo::IsWindowed() const
{
	return _hasWindow;
}

int ProcessInfo::Init()
{
	const auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _prEntry->th32ProcessID);
	// If the function fails, the return value is NULL
	if (hProcess == nullptr)
	{
		const auto errC = GetLastError();
		CloseHandle(hProcess);
		//If the specified process is the System Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
		if (_prEntry->th32ProcessID == 0 && errC == ERROR_INVALID_PARAMETER)
		{
			// Skip System Process
			return GetLastError();
		}
		/*
		* If the specified process is the Idle process or one of the CSRSS processes,
		* this function fails and the last error code is ERROR_ACCESS_DENIED
		* because their access restrictions prevent user-level code from opening them.
		*/
		if (errC == ERROR_ACCESS_DENIED)
		{
			// Skip Idle or CSRSS processes
			return GetLastError();
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
		CloseHandle(hProcess);
		return GetLastError();
	}
	CloseHandle(hProcess);
	_fullPathToExe = szModName;

	// Enumerate threads of this process
	auto hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		auto err = GetLastError();
		CloseHandle(hThreadSnap);          // clean the snapshot object
		return err;
	}
	_windows.pid = _prEntry->th32ProcessID;
	// Enum Windows for main PID
	EnumWindows(EnumerateWindowsHandlers, reinterpret_cast<LPARAM>(&_windows));

	if (_windows.handles.size() > 0)
	{
		_tprintf(TEXT("\nMAIN WINDOW      = %s"), _windows.handles.at(0).windowTitle.c_str());
	}
	_tprintf(TEXT("\nMAIN NAME        = %s"), _prEntry->szExeFile);
	_tprintf(TEXT("\nMAIN PATH        = %s"), _fullPathToExe.c_str());
	_tprintf(TEXT("\nMAIN THREAD      = 0x%08X"), _prEntry->th32ProcessID);
	_tprintf(TEXT("\nMAIN THCNT       = %d"), _prEntry->cntThreads);

	// Now walk the thread list 
	do
	{
		if (te32.th32OwnerProcessID == _prEntry->th32ProcessID)
		{
			_tprintf(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
			_tprintf(TEXT("\n     Base priority  = %d"), te32.tpBasePri);
			_tprintf(TEXT("\n     Delta priority = %d"), te32.tpDeltaPri);
			_tprintf(TEXT("\n"));
		}
	} while (Thread32Next(hThreadSnap, &te32));



	CloseHandle(hThreadSnap);
	return(TRUE);




	//// Get file information

	//// Get file info size
	//for (auto &&pair : _applicationInfoMap)
	//{
	//	//If the function fails, the return value is zero.
	//	const auto retSize = GetFileVersionInfoSize(pair.second->GetPath().c_str(), nullptr);
	//	if (!retSize)
	//	{
	//		return GetLastError();
	//	}
	//	auto pBlock = new DWORD[retSize / sizeof(DWORD)];
	//	//If the function fails, the return value is zero.
	//	if (!GetFileVersionInfo(pair.second->GetPath().c_str(), NULL, retSize, pBlock))
	//	{
	//		delete[] pBlock;
	//		return GetLastError();
	//	}

	//	const TCHAR *paramNames[] = {
	//		_T("FileDescription"),
	//		_T("CompanyName"),
	//		_T("FileVersion"),
	//		_T("InternalName"),
	//		_T("LegalCopyright"),
	//		_T("LegalTradeMarks"),
	//		_T("OriginalFilename"),
	//		_T("ProductName"),
	//		_T("ProductVersion"),
	//		_T("Comments"),
	//		_T("Author")
	//	};

	//	struct LANGANDCODEPAGE {
	//		WORD wLanguage;
	//		WORD wCodePage;
	//	} *pLangCodePage;

	//	UINT cpSz;

	//	if (!VerQueryValue(pBlock, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&pLangCodePage), &cpSz))
	//	{
	//		delete[] pBlock;
	//		return GetLastError();
	//	}


	//	TCHAR paramNameBuf[256]; // здесь формируем имя параметра
	//	TCHAR *paramValue;       // здесь будет указатель на значение параметра, который нам вернет функция VerQueryValue
	//	UINT paramSz;            // здесь будет длина значения параметра, который нам вернет функция VerQueryValue

	//	for (int cpIdx = 0; cpIdx < (int)(cpSz / sizeof(struct LANGANDCODEPAGE)); cpIdx++)
	//	{
	//		// перебираем имена параметров
	//		for (int paramIdx = 0; paramIdx < sizeof(paramNames) / sizeof(char*); paramIdx++)
	//		{
	//			// формируем имя параметра ( \StringFileInfo\кодовая_страница\имя_параметра )
	//			_stprintf(paramNameBuf, _T("\\StringFileInfo\\%04x%04x\\%s"),
	//				pLangCodePage[cpIdx].wLanguage,  // ну, или можно сделать фильтр для
	//				pLangCodePage[cpIdx].wCodePage,  // какой-то отдельной кодовой страницы
	//				paramNames[paramIdx]);

	//			if (VerQueryValue(pBlock, paramNameBuf, (LPVOID*)&paramValue, &paramSz))
	//				std::wcout << L"\t\t" << paramNames[paramIdx] << L":\t\t" << paramValue << std::endl;
	//			else
	//				std::wcout << L"\t\t" << paramNames[paramIdx] << L"\t\tHет информации " << std::endl;
	//		}
	//	}
	//	delete[] pBlock;
	//}
	return 0;
}

BOOL ProcessInfo::EnumerateWindowsHandlers(const HWND hwnd, const LPARAM l_param)
{
	DWORD lpdwProcessId;
	auto args = reinterpret_cast<EnumWindowsCallbackArgs *>(l_param);
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	WindowInfo winInfo;
	if (lpdwProcessId == args->pid) {
		//If the window has no text, the return value is zero.
		const auto titleLength = GetWindowTextLength(hwnd);
		if (titleLength == 0)
		{
			winInfo.windowTitle = std::wstring(L"Unknown");
			return TRUE;
		}
		const auto strBuffer = new TCHAR[titleLength + 1];

		//If the window has no text, the return value is zero.
		const auto retLength = GetWindowText(hwnd, strBuffer, titleLength + 1);
		if (retLength == 0)
		{
			winInfo.windowTitle = std::wstring(L"Unknown");
			delete[]strBuffer;
			return TRUE;
		}
		winInfo.windowTitle = std::wstring(strBuffer);
		delete[]strBuffer;
		args->handles.emplace_back(winInfo);
		return FALSE;
	}
	return TRUE;
}