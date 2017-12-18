#include "AppStatGrabber.h"
#include "../../Misc/RtCrc32.h"


AppStatGrabber::AppStatGrabber()
{
}

int AppStatGrabber::Update()
{

	const auto activeWnd = GetForegroundWindow();
	if (activeWnd == nullptr)
	{
		// no active windows
		return GetLastError();
	}

	DWORD processId;
	auto threadId = GetWindowThreadProcessId(activeWnd, &processId);

	const auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
	// If the function fails, the return value is NULL
	if (hProcess == nullptr)
	{
		const auto errC = GetLastError();
		CloseHandle(hProcess);
		//If the specified process is the System Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
		if (processId == 0 && errC == ERROR_INVALID_PARAMETER)
		{
			// Skip System Process
			return errC;
		}
		/*
		* If the specified process is the Idle process or one of the CSRSS processes,
		* this function fails and the last error code is ERROR_ACCESS_DENIED
		* because their access restrictions prevent user-level code from opening them.
		*/
		if (errC == ERROR_ACCESS_DENIED)
		{
			// Skip Idle or CSRSS processes
			return errC;
		}
		// 
		return errC;
	}

	//  fully qualified path for the file containing the specified module
	TCHAR _szModName[MAX_PATH];

	// If the function fails, the return value is zero.
	if (GetModuleFileNameEx(hProcess, nullptr, _szModName, sizeof(_szModName) / sizeof(TCHAR)) == 0)
	{
		CloseHandle(hProcess);
		return GetLastError();
	}
	CloseHandle(hProcess);

	// calc crc32 hash of full path
	Crc32Hasher crc32Hasher;

	// crc32 of _szModName xor process id in current session
	auto _id = crc32Hasher.GetHashCode(_szModName, MAX_PATH);
	// xor it with process id
	_id ^= processId;

	/* Print to console */
	/*std::wstringstream sscrc32, ssid;*/
	//sscrc32 << std::setw(8) << std::left << L"CRC32:" << std::right << L"0x" << std::setfill(L'0') << std::uppercase << std::setw(8) << std::hex << crc32 << std::endl;
	//ssid << std::setw(8) << std::left << L"ID:" << L"0x" << std::right << std::setfill(L'0') << std::uppercase << std::setw(8) << std::hex << processId << std::endl;
	//std::wcout << sscrc32.str() << ssid.str() << std::setw(8) << std::left << L"Path: " << szModName << std::endl;
}


AppStatGrabber::~AppStatGrabber()
{
}
