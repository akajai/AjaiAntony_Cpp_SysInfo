#include "stdafx.h"
#include "ClientUtils.h"

/*
Constructor
*/

ClientUtils::ClientUtils()
{
}
/*
Converts server IP to http://serverIP:8080/Update and update the member variabel
*/

int ClientUtils::InitHttpUrl(string serverIp) {
	try
	{

		httpurl = this->HTTP + serverIp + this->URL;

	}
	catch (const std::exception&ex)
	{
		cout << "Exception is InitHttpUrl " << serverIp << ex.what() << endl;
		return -1;
	}
	return 0;
}
/*
Retrives the url for sending resource data. The url is processed in InitHttpUrl function
*/
string ClientUtils::getHttpUrl() {
	try
	{
		return httpurl;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is getHttpUrl " << ex.what() << endl;
		return "";
	}
	return httpurl;
}
/*
getHostName(); This is the client key in the xml. This gives each pc in intranet a unique identification

*/
string ClientUtils::getHostName() {
	try
	{
		TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = sizeof(computerName) / sizeof(computerName[0]);
		GetComputerName(computerName, &size);
		wstring test(&computerName[0]);
		string hostname(test.begin(), test.end());
		this->hostname = hostname;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is getHostName " << ex.what() << endl;
		this->hostname = "";
	}
	return this->hostname;
}
/*
The RAM usage is calculate using windows api and updated in membervariable and returned
*/
double ClientUtils::GetMemoryUsage() {
	try
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG ullAvailPhys = memInfo.ullAvailPhys;
		DWORDLONG ullTotalPhys = memInfo.ullTotalPhys;
		this->dmemusageper = (ullTotalPhys - ullAvailPhys) * 100 / ullTotalPhys;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is GetMemoryUsage " << ex.what() << endl;
		this->dmemusageper = -1;
	}
	return this->dmemusageper;
}

/*
Get the number of process  used ,using windows api and updated in membervariable and returned
*/
double ClientUtils::GetProcessCount() {
	try
	{
		DWORD aProcesses[1024], cbNeeded;
		if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		{
			return -1;
		}
		// Calculate how many process identifiers were returned.
		this->dprocesses = cbNeeded / sizeof(DWORD);
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is GetProcessCount " << ex.what() << endl;
		this->dprocesses = -1;
	}
	return this->dprocesses;
}
/*
THis gets the CPU usage by calling private function CalculateCPULoad and conver it to %
*/
float ClientUtils::GetCpuUsage() {
	try
	{
		FILETIME idleTime, kernelTime, userTime;
		double retValue = GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
		this->fcpuusage = retValue * 100;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is GetCpuUsage " << ex.what() << endl;
		this->fcpuusage = -1;
	}
	return this->fcpuusage;
}
/*
Logs the system respouce parameters like hostname,RAM,CPU,Process usage to the log file 
*/
int ClientUtils::LogInfo() {
	try {
		string logMessage;
		logMessage = "HostName  : " + this->getHostName() + ", MemoryUsage : " + to_string(this->GetMemoryUsage()) + ", CpuUsage : " + to_string(this->GetCpuUsage()) + ", ProcessCount : " + to_string(this->GetProcessCount());
		std::ofstream logFile;
		logFile.open(this->LOGFILENAME, std::ofstream::out | std::ofstream::app);
		logFile << logMessage << std::endl;
		logFile.close();
		cout << logMessage << endl;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is LogInfo " << ex.what() << endl;
		return -1;
	}

	return 0;
}
/*
Logs the error message to the log file
*/
int ClientUtils::LogInfo(string logMessage)
{
	try {
		std::ofstream logFile;
		logFile.open(this->LOGFILENAME, std::ofstream::out | std::ofstream::app);
		logFile << logMessage << std::endl;
		logFile.close();
		cout << logMessage << endl;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is LogInfo " << ex.what() << endl;
		return -1;
	}

	return 0;
}


/*
Calculate the CPU load.
*/
float ClientUtils::CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	try
	{
		static unsigned long long _previousTotalTicks = 0;
		static unsigned long long _previousIdleTicks = 0;

		unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
		unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

		float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

		_previousTotalTicks = totalTicks;
		_previousIdleTicks = idleTicks;
		return ret;
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is CalculateCPULoad " << ex.what() << endl;
		return -1;
	}

}
/*
Calculate the FileTimeToInt64.
*/
unsigned  long ClientUtils::FileTimeToInt64(const FILETIME & ft)
{
	return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}
//
/*
Retrive the 5 minute sleep interval calculated in milliseconds
*/
long  ClientUtils::getSleepInterval()
{
	return lsleepInterval;
}

ClientUtils::~ClientUtils()
{
}
