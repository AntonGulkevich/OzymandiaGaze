#pragma once

#include <Windows.h> // winapi
#include <sstream> // wstringstream
#include <chrono> // datetime 
#include <map> // std::map
#include <thread> //std::thread
#include <mutex> // std::mutex
#include <psapi.h> // GetModuleFileNameEx
#include <tchar.h> // _T macro
#include <iostream> //
#include <atomic> //

#include "../../Misc/RtCrc32.h"

class PeriodicThread
{
public:

	enum class PeriodicThreadState;

private:
	// general specialization unsupported
	template <typename T>
	T GetLastExecutionTime();

public:

	// D-tor
	virtual ~PeriodicThread()
	{
		Stop();
		if (_th.joinable())
			_th.join();
	}

	// specialization for microseconds
	template <>
	std::chrono::microseconds GetLastExecutionTime()
	{
		return {};
	}

	// specialization for milliseconds
	template <>
	std::chrono::milliseconds GetLastExecutionTime()
	{
		return {};
	}

	PeriodicThreadState GetState()
	{
		std::unique_lock<std::mutex> lc(_mtx);
		return _state;
	}

	unsigned Start()
	{
		std::unique_lock<std::mutex> lc(_mtx);
		_state = PeriodicThreadState::starting;
		_th = std::thread(&PeriodicThread::Cycle, this);
		return 0;
	}

	unsigned Stop()
	{
		{
			std::unique_lock<std::mutex> lc(_mtx);
			_stopped = true;
			_paused = false;
			_cv.notify_one();
		}
		return 0;
	}
	unsigned Pause()
	{
		{
			std::unique_lock<std::mutex> lc(_mtx);
			_paused = true;
		}
		return 0;
	}
	unsigned Resume()
	{
		{
			std::unique_lock<std::mutex> lc(_mtx);
			_paused = false;
			_cv.notify_one();
		}
		return 0;
	}

	enum class PeriodicThreadState
	{
		ready,
		paused,
		running,
		starting,
		stopping,
		stopped,

	};

private:

	// main thread
	std::thread _th{};

	// basic mutex
	std::mutex _mtx{};

	// cv for pause/resume
	std::condition_variable _cv;

	// 
	std::atomic_bool _stopped{ true };

	//
	std::atomic_bool _paused{ false };

	PeriodicThreadState _state{PeriodicThreadState::ready};

	void Cycle()
	{
		while (!_stopped)
		{
			while (_paused)
			{
				std::unique_lock<std::mutex> lc(_mtx);
				_cv.wait(lc);
			}
			if (_stopped)
				return;
			Run();

		}
	}

	virtual void Run() = 0;


};

class ActiveWindowTracker : public PeriodicThread
{
private:
	void Run() override
	{
		std::wcout << L"UpdateActiveWindowHandler" << std::endl;
	};
};


class AppStatGrabber
{
public:

	enum class State
	{
		ready,
		paused,
		running,
		starting,
		stopping,
		stopped
	};

	using ACTIVE_PR_ID = unsigned long;

public:

	// C-tor
	AppStatGrabber()
	{

	}
	// D-tor
	virtual ~AppStatGrabber()
	{
		//
		Stop();


		// join
		_activeWindowTracker.join();
	};

	// 
	unsigned Start()
	{
		if (_currentState != State::stopped)
			return 1; // INVALID_STATE

		// initialize Active Window Tracker thread
		_activeWindowTracker = std::thread(&AppStatGrabber::UpdateActiveWindowHandler, this);



		return 0;
	}
	unsigned Stop();
	unsigned Pause();
	unsigned Resume();

private:

	// dicionary contains identifier of active prorgamm and info about its content
	std::map<ACTIVE_PR_ID, std::shared_ptr<int>> _infoMap;

	//
	std::thread _activeWindowTracker{};

	//
	std::mutex _mutex{};

	// 
	State _currentState{ State::ready };

	// hasher
	Crc32Hasher _crc32Hasher;

	//
	std::condition_variable _cv{};


private:

	// return 0 on success, error code on failure
	unsigned UpdateActiveWindowHandler()
	{
		//while (true)
		//{
		//	// get hadle of current active window
		//	const auto activeWnd = GetForegroundWindow();
		//	if (activeWnd == nullptr)
		//	{
		//		// no active windows
		//		return GetLastError();
		//	}

		//	// threadId value is the identifier of the thread that created the window
		//	DWORD processId;
		//	const auto threadId = GetWindowThreadProcessId(activeWnd, &processId);

		//	//
		//	const auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
		//	// If the function fails, the return value is NULL
		//	if (hProcess == nullptr)
		//	{
		//		const auto errC = GetLastError();
		//		//If the specified process is the System Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
		//		if (processId == 0 && errC == ERROR_INVALID_PARAMETER)
		//		{
		//			// Skip System Process
		//			return errC;
		//		}
		//		/*
		//		* If the specified process is the Idle process or one of the CSRSS processes,
		//		* this function fails and the last error code is ERROR_ACCESS_DENIED
		//		* because their access restrictions prevent user-level code from opening them.
		//		*/
		//		if (errC == ERROR_ACCESS_DENIED)
		//		{
		//			// Skip Idle or CSRSS processes
		//			return errC;
		//		}
		//		return errC;
		//	}

		//	// If the function fails, the return value is zero.
		//	if (!GetModuleFileNameEx(hProcess, nullptr, __lpFilename, MAX_PATH))
		//	{
		//		CloseHandle(hProcess);
		//		return GetLastError();
		//	}
		//	CloseHandle(hProcess);


		//	// crc32 of _szModName xor process id in current session
		//	auto id = processId ^ _crc32Hasher.GetHashCode(__lpFilename, MAX_PATH);

		//	
		//}
		return 0;
	}

public:

private:

	//  fully qualified path for the file containing the specified module
	TCHAR _lpFilename[MAX_PATH];
};

//#include "ApplicationStatistics.h"
//#pragma  comment(lib, "Version.lib") // GetFileVersionInfoSize

//int ProcessInfo::InitWndThree()
//{
//	const auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, _prEntry->th32ProcessID);
//	// If the function fails, the return value is NULL
//	if (hProcess == nullptr)
//	{
//		const auto errC = GetLastError();
//		CloseHandle(hProcess);
//		//If the specified process is the System Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
//		if (_prEntry->th32ProcessID == 0 && errC == ERROR_INVALID_PARAMETER)
//		{
//			// Skip System Process
//			return GetLastError();
//		}
//		/*
//		* If the specified process is the Idle process or one of the CSRSS processes,
//		* this function fails and the last error code is ERROR_ACCESS_DENIED
//		* because their access restrictions prevent user-level code from opening them.
//		*/
//		if (errC == ERROR_ACCESS_DENIED)
//		{
//			// Skip Idle or CSRSS processes
//			return GetLastError();
//		}
//		// 
//		return errC;
//LPTSTR messageBuffer = nullptr;
//const size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//                                   nullptr, errC, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&messageBuffer), 0, nullptr);
//const std::wstring message(messageBuffer, size);
//tmpApplInfo->SetPath(message.c_str());
////Free the buffer.
//LocalFree(messageBuffer);
//	}
//
//#pragma region Enumerate threads of this process
//// Enumerate threads of this process
//auto hThreadSnap = INVALID_HANDLE_VALUE;
//THREADENTRY32 te32;

//// Take a snapshot of all running threads  
//hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
//if (hThreadSnap == INVALID_HANDLE_VALUE)
//{
//	return GetLastError();
//}

//// Fill in the size of the structure before using it. 
//te32.dwSize = sizeof(THREADENTRY32);

//// Retrieve information about the first thread,
//// and exit if unsuccessful
//if (!Thread32First(hThreadSnap, &te32))
//{
//	auto err = GetLastError();
//	CloseHandle(hThreadSnap);          // clean the snapshot object
//	return err;
//}

//// Now walk the thread list 
//do
//{
//	if (te32.th32OwnerProcessID == _prEntry->th32ProcessID)
//	{
//		_tprintf(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
//		_tprintf(TEXT("\n     Base priority  = %d"), te32.tpBasePri);
//		_tprintf(TEXT("\n     Delta priority = %d"), te32.tpDeltaPri);
//		_tprintf(TEXT("\n"));
//	}
//} while (Thread32Next(hThreadSnap, &te32));

//#pragma endregion
//
//	// Enum Windows for main PID
//	_windowsInfoTree._maxDeep = 2;
//	EnumWindows(EnumerateWindowsHandlers, reinterpret_cast<LPARAM>(&_windowsInfoTree));
//
//	return 0;
//}
//
//BOOL ProcessInfo::EnumerateWindowsHandlers(const HWND hwnd, LPARAM l_param)
//{
//	DWORD lpdwProcessId;
//	auto args = reinterpret_cast<WinTreeInfo *>(l_param);
//	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
//	if (lpdwProcessId == args->processId) {
//		auto winInfo = new WindowInfo;
//		winInfo->hwnd = hwnd;
//		winInfo->isActive = false;
//		winInfo->maxZorder = args->_maxDeep;
//		//If the window has no text, the return value is zero.
//		const auto titleLength = GetWindowTextLength(hwnd);
//		if (titleLength)
//		{
//			const auto strBuffer = new TCHAR[titleLength + 1];
//			const auto retLength = GetWindowText(hwnd, strBuffer, titleLength + 1);
//			if (retLength)
//				winInfo->window_title = std::wstring(strBuffer);
//			delete[]strBuffer;
//		}
//		args->_windowInfoTree.emplace_back(winInfo);
//		
//		EnumChildWindows(hwnd, EnumerateChildWindowsHandlers, reinterpret_cast<LPARAM>(winInfo));
//	}
//	return TRUE;
//}
//
//BOOL ProcessInfo::EnumerateChildWindowsHandlers(HWND hwnd, LPARAM lParam)
//{
//	if (hwnd) {
//		auto args = reinterpret_cast<WindowInfo *>(lParam);
//
//		if (args->zOrder + 1 > args->maxZorder)
//			return FALSE;
//		auto winInfo = new WindowInfo;
//		winInfo->maxZorder = args->maxZorder;
//		winInfo->hwnd = hwnd;
//		winInfo->isActive = false;
//		winInfo->parentHwnd = args->parentHwnd;
//		winInfo->zOrder = args->zOrder + 1;
//		//If the window has no text, the return value is zero.
//		const auto titleLength = GetWindowTextLength(hwnd);
//		if (titleLength)
//		{
//			const auto strBuffer = new TCHAR[titleLength + 1];
//			const auto retLength = GetWindowText(hwnd, strBuffer, titleLength + 1);
//			if (retLength)
//				winInfo->window_title = std::wstring(strBuffer);
//			delete[]strBuffer;
//		}
//
//		args->childWindowsVector.emplace_back(winInfo);
//		EnumChildWindows(hwnd, EnumerateChildWindowsHandlers, reinterpret_cast<LPARAM>(winInfo));
//	}
//	return TRUE;
//}
//
//DWORD ProcessInfo::GetProcessId() const
//{
//	return _prEntry->th32ProcessID;
//}

//
//struct WindowInfo
//{
//	HWND parentHwnd = nullptr;
//	HWND hwnd = nullptr;
//	bool isActive = false;
//	std::wstring window_title = std::wstring(TEXT("Unknown"));
//	std::vector<WindowInfo*> childWindowsVector;
//	UINT zOrder = 0;
//	UINT maxZorder = 0;
//	friend std::wostream& operator<<(std::wostream& os, const WindowInfo& window_info)
//	{
//		os << std::right << std::setw(window_info.zOrder * 2) << L"|" << L" WND NAME: " << window_info.window_title.c_str() << std::endl;
//		for (auto child_wnd_info : window_info.childWindowsVector)
//		{
//			os << *child_wnd_info;
//		}
//		return os;
//	}
//};
//
//struct WinTreeInfo
//{
//	explicit WinTreeInfo(DWORD pid) : processId(pid), _maxDeep(0)
//	{
//	}
//
//	// parent PID
//	DWORD processId;
//	// Tree of windows
//	std::vector<WindowInfo*> _windowInfoTree;
//	// Max deep of child windows
//	DWORD _maxDeep;
//
//	friend std::wostream& operator<<(std::wostream& os, const WinTreeInfo& win_tree_info)
//	{
//		for (auto window_info : win_tree_info._windowInfoTree)
//			os << *window_info;
//		return os;
//	}
//};
//
//
//INT ApplicationStatistics::InitializeApplInfo()
//{
//	// Take a snapshot of all processes in the system.
//	const auto hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	// If the function fails, it returns INVALID_HANDLE_VALUE.
//	if (hProcessSnap == INVALID_HANDLE_VALUE)
//	{
//		CloseHandle(hProcessSnap);
//		return GetLastError();
//	}
//
//	PROCESSENTRY32 pe32;
//	// Set the size of the structure before using it.
//	pe32.dwSize = sizeof(PROCESSENTRY32);
//	// Retrieve information about the first process and exit if unsuccessful
//	if (!Process32First(hProcessSnap, &pe32))
//	{
//		CloseHandle(hProcessSnap); // clean the snapshot object
//		return GetLastError();
//	}
//
//	// Walk the snapshot of processes
//	do
//	{
//		// Create a new instance of ProcessInfo using uniqu_ptr
//		auto tmpApplInfo = std::make_unique<ProcessInfo>(pe32);
//
//		auto err = tmpApplInfo->InitWndThree();
//		if (err)
//			Process32Next(hProcessSnap, &pe32);
//	} while (Process32Next(hProcessSnap, &pe32));
//	// Close hProcessSnap
//	CloseHandle(hProcessSnap);
//
//	return 0;
//}
//
