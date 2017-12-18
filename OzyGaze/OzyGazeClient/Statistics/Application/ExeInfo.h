#pragma once
#include <Windows.h>
#include <tchar.h> // _T macro
#include <chrono> // std::chrono::

class ExeInfo
{
public:

	typedef std::chrono::time_point<std::chrono::system_clock> time_t;

private:

	time_t _creationDate;
	time_t _ñhangeDate;

public:

	ExeInfo();
	~ExeInfo();

	errno_t Init(TCHAR * _path);
};

