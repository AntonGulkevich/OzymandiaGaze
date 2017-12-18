#pragma once

/*
 *
 *	Contains information about application:
 *	
 *	1. time info:
 *		list of date with infos:
 *			date of start
 *			all execution time
 *			time spent in foreground mode
 *			
 *	2. exe info:
 *		full path
 *		size
 *		version
 *		manufacturer
 *		crc32
 *		
 */

#include <Windows.h>
#include <psapi.h> // Windows process API
#include <tlhelp32.h> // CreateToolhelp32Snapshot

/* STL input/oputput streams */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

/* STL containers*/
#include <map>
#include <vector>
#include <mutex>

#include "ProcessInfo.h"
#include "../../Misc/RtCrc32.h" /// crc 32

#pragma  comment(lib, "Version.lib") // GetFileVersionInfoSize


class ApplicationStatistics
{
public:
	virtual ~ApplicationStatistics(){};
	INT InitializeApplInfo();
private:
	//
	ProcessInfo _applicationInfo;
	// Average count of precesses
	DWORD _averageCountOfProcesses;
	// Max count of processes
	DWORD _maxCountOfProcesses;
	// Min count of processes
	DWORD _minCountOfProcesses;
	// The last count of processes
	DWORD _lastCountOfProcesses;

	// Foreground wnd info map
};

