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
*	2. process info: from ProcessInfo.h
*		full path
*		size
*		version
*		manufacturer
*		crc32
*
*/


#include "ProcessInfo.h"

#include <sstream> // wstringstream
#include <chrono> // datetime 
#include <map> // std::map

class AppTimeStamp
{
private:
	
public:

};

class AppStatGrabber
{
public:

	typedef unsigned long id_t;

public:

	AppStatGrabber();

	// return 0 on success, error code on failure
	int Update();

	void Start();
	void Stop();


	virtual ~AppStatGrabber();

private:
	std::map<id_t, std::unique_ptr<ProcessInfo>> _infoMap;

};

