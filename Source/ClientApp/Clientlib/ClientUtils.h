#pragma once
#include "stdafx.h"


class ClientUtils
{
private:
	const string LOGFILENAME = "srm.log"; //log file name, harcoded
	const string HTTP = "http://";			// http part of url 
	const string URL = ":8080/Update";		// Update and port part of URL
	string strserverIP = "localhost";		// Default server IP
	string httpurl = "";					// Fully formated URL member variable
	string hostname;						// Comuter name or uniqu key to represent a statiom
	double dmemusageper = 0;				//RAM usage
	double dprocesses = 0;					//Process Count
	float fcpuusage = 0;					//	CPU Usage hoding variable
	float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
	unsigned  long FileTimeToInt64(const FILETIME & ft);
	const long lsleepInterval = (.5 * 60 * 1000); // 5 min* 60  sec*1000 milli sec

public:
	ClientUtils();
	int InitHttpUrl(string serverIp);
	string getHttpUrl();
	string getHostName();
	double GetMemoryUsage();
	double GetProcessCount();
	float GetCpuUsage();
	int LogInfo();
	int LogInfo(string logMessage);
	long getSleepInterval();
	~ClientUtils();
};

