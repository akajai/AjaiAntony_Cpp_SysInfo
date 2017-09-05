#pragma once

#include "stdafx.h"



using namespace std;
class Client
{
public:
	Client();
	~Client();
	Client(unsigned int id_) :id(id_) {}

	unsigned int id = 0;
	string key = "";
	string mail = "";
	string memory_per = "";
	string cpu_per = "";
	double memory = 0;
	double cpu = 0;
	int process_count = 0;
};

class Utils
{

private:
	std::list< Client*> clientList;
	SQLHANDLE sqlconnectionhandle = NULL;
	SQLHANDLE sqlstatementhandle = NULL;
	SQLWCHAR connstring[255] = L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=NBMSCSDB;UID=sa;PWD=root;";
	string emailTemplateHTML;
	const string mailhost = "smtp.gmail.com";
	const string sender = "alertmailcrossover@gmail.com";
	const string username = "alertmailcrossover@gmail.com";
	const string password = "MailServer";

public:
	Utils();
	int  loadXML(std::string filename);
	std::list< Client*> getClientList();
	int SQLInit();
	void show_error(unsigned int handletype, const SQLHANDLE& handle);
	int LogDB(string clientkey, double mem_usage, double cpu_usage, double processcount);
	int SQLFree();
	int RuleEngine(string strkey, double ram, double cpu, int process);
	int LoadMailTemplate(string emailTemplate);

	int SendMail(string strkey, string recipientemail, double dramusage, double  dcpuusage, double  dprocessusage, double  ramusagelmt, double  cpuusagelmt, int  processusagelmt);

	~Utils();
};

