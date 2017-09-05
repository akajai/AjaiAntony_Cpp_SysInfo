#include "stdafx.h"
#include "Utils.h"

#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/SharedPtr.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include <Poco\Net\StringPartSource.h>
#include <Poco\Net\FilePartSource.h>
#include <Poco\Net\MailMessage.h>

using boost::property_tree::ptree;
using Poco::Net::MailMessage;
using Poco::Net::MailRecipient;
using Poco::Net::SMTPClientSession;
using Poco::Net::SecureSMTPClientSession;
using Poco::Net::StringPartSource;
using Poco::Net::SSLManager;
using Poco::Net::Context;
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::ConsoleCertificateHandler;
using Poco::SharedPtr;
using Poco::Path;
using Poco::Exception;
/*
Constant used for transversing tree
*/
const ptree & empty_ptree() {
	static ptree t;
	return t;
}
/* Constructor
*/
Utils::Utils()
{
}
/*
Loads the xml and parse it and fill it to client list. This has the list of clients that are monitored.
Their hostname , email to be used for sending the mail and the limit parameters
*/
int Utils::loadXML(std::string filename)
{
	try
	{
		Client *client;

		int clientindex = 0;
		ptree tree;
		read_xml(filename, tree);
		const ptree & formats = tree.get_child("clients", empty_ptree());
		BOOST_FOREACH(const ptree::value_type & node, formats) {
			client = new Client(clientindex);
			if (node.first == "client") {
				boost::property_tree::ptree subtree = node.second;
				BOOST_FOREACH(boost::property_tree::ptree::value_type const& node1, subtree.get_child(""))
				{
					std::string label = node1.first;
					if (label != "<xmlattr>") {
						//sstd::cout << label << ":  " << node1.second.get_child("").data() << endl;
						const ptree & attributes = node1.second.get_child("<xmlattr>", empty_ptree());
						//cout << "Extracting attributes from " << at << ":" << endl;
						string prevVal = "";
						BOOST_FOREACH(const ptree::value_type &v, attributes) {
							//	cout << "key: " << v.first.data() << " Value: " << v.second.data() << endl;
							if ("memory" == prevVal) {
								client->memory_per = v.second.data();
								client->memory = stod(client->memory_per);
							}
							else if ("cpu" == prevVal) {
								client->cpu_per = v.second.data();
								client->cpu = stod(client->cpu_per);
							}
							else if ("processes" == prevVal) {
								client->process_count = std::stod(v.second.data());
							}
							else {
								prevVal = v.second.data();
							}
						}
					}

				}
			}
			string at = node.first;
			const ptree & attributes = node.second.get_child("<xmlattr>", empty_ptree());
			//		cout << "Extracting attributes from " << at << ":" << endl;
			string keyVal = "";
			BOOST_FOREACH(const ptree::value_type &v, attributes) {
				//		cout << "key: " << v.first.data() << " Value: " << v.second.data() << endl;
				keyVal = v.first.data();
				if ("key" == keyVal) {
					client->key = v.second.data();
				}
				else if ("mail" == keyVal) {
					client->mail = v.second.data();
				}

			}
			clientList.push_front(client);
			clientindex++;
		}
	}
	catch (const std::exception& ex)
	{
		cout << "Exception in XML Parsing " << ex.what() << endl;
		return -1;
	}

	return 0;
}
/*
Get the list representation of the XML
*/

std::list< Client*>Utils::getClientList() {
	return clientList;
}
/*
Initialize the sql variable
*/

int Utils::SQLInit() {
	SQLHANDLE sqlenvhandle;
	SQLRETURN retcode;
	retcode = -1;
	try
	{
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
			return -1;

		if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			return -1;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
			return -1;

		SQLWCHAR retconstring[1024];
		switch (SQLDriverConnect(sqlconnectionhandle, NULL,
			connstring,
			SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT))
		{
		case SQL_SUCCESS_WITH_INFO:
			retcode = 0;
			show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
			break;
		case SQL_INVALID_HANDLE:
		case SQL_ERROR:
			show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
			retcode = -1;
			break;
		default:
			break;
		}
	}
	catch (const std::exception& ex)
	{
		cout << "Exception in MSSQL Initialization " << ex.what() << endl;
		retcode = -1;
	}
	return retcode;

}
/*
Display the SQL handlers value
*/
void Utils::show_error(unsigned int handletype, const SQLHANDLE& handle) {
	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
		cout << "Message: " << message << "\nSQLSTATE: " << sqlstate << endl;

}
/*
Insert the client parameters to the DB
*/
int Utils::LogDB(string clientkey, double mem_usage, double cpu_usage, double processcount) {
	try
	{
		wstring wclientkey(clientkey.begin(), clientkey.end());
		wstring query = L"INSERT INTO [NBMSCS_log] ([client_key] ,[ram] ,[cpu] ,[process] ) VALUES ( '" + wclientkey + L"'," + to_wstring(mem_usage) + L"," + to_wstring(cpu_usage) + L"," + to_wstring(processcount) + L")";
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
			return -1;
		if (SQL_SUCCESS == SQLExecDirect(sqlstatementhandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
			return 0;
		}
	}
	catch (const std::exception&ex)
	{
		cout << "Exception in MSSQL LogDB " << ex.what() << endl;
		return -1;
	}
	return 0;
}
/*
Free the DB handlers
*/
int  Utils::SQLFree() {

	try
	{
		SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
		SQLDisconnect(sqlconnectionhandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);

	}
	catch (const std::exception&ex)
	{
		cout << "Exception in MSSQL SQLFree " << ex.what() << endl;
		return -1;
	}
	return 0;
}
/*
Rule Engine
This checks for violation or match of client value with xml values.
If there is a condition triggered a mail is send And a console is logged.

*/
int Utils::RuleEngine(string strkey, double dramusage, double dcpuusage, int dprocessusage) {
	try
	{
		double ramusagelmt = 0, cpuusagelmt = 0;
		int processusagelmt = 0;
		string email = "";
		list<Client*>clientList = getClientList();
		list<Client*>::iterator it;
		for (it = clientList.begin(); it != clientList.end(); it++)
		{
			bool bsendAlert = false;
			ramusagelmt = (*it)->memory;
			cpuusagelmt = (*it)->cpu;
			processusagelmt = (*it)->process_count;
			email = (*it)->mail;
			if (strkey.compare((*it)->key) == 0) {

				if (dramusage >= ramusagelmt) {
					bsendAlert = true;
				}
				if (dcpuusage >= cpuusagelmt) {
					bsendAlert = true;
				}
				if (dprocessusage >= processusagelmt) {
					bsendAlert = true;

				}
			}
			if (bsendAlert) {
				cout << "Alert triggered for  " << strkey << endl;
				this->SendMail(strkey, email, dramusage, dcpuusage, dprocessusage, ramusagelmt, cpuusagelmt, processusagelmt);
			}

		}
	}
	catch (const std::exception&ex)
	{
		cout << "Exception in Rule Engines " << ex.what() << endl;
		return -1;
	}


	return 0;
}
/*
Load the email  template to the string 
*/
int Utils::LoadMailTemplate(string emailTemplate) {
	try {
		std::ifstream ifs(emailTemplate);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		this->emailTemplateHTML = content;

	}
	catch (const std::exception&ex)
	{
		cout << "Exception is LoadMailTemplate  " << ex.what() << endl;
		return -1;
	}
	if (emailTemplateHTML.size() > 0) {
		return 0;
	}
	else {
		return -1;
	}

}
/*
Send mail parses the email template string , fill it  with data and sends it. The email provider
data is stored in constants private 
*/
int Utils::SendMail(string strkey, string recipientemail, double dramusage, double  dcpuusage, double  dprocessusage, double  ramusagelmt, double  cpuusagelmt, int  processusagelmt)
{
	try
	{
		Poco::Net::initializeSSL();

		// Note: we must create the passphrase handler prior Context 
		SharedPtr<InvalidCertificateHandler> pCert = new ConsoleCertificateHandler(false); // ask the user via console
		Context::Ptr pContext = new Context(Context::CLIENT_USE, "");
		SSLManager::instance().initializeClient(0, pCert, pContext);

		MailMessage message;
		message.setSender(this->sender);
		message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recipientemail));
		message.setSubject("Alert Report");
		std::string content;
		content = this->emailTemplateHTML;

		boost::replace_all(content, "{#KEY}", strkey);
		boost::replace_all(content, "{#MAILD}", recipientemail);
		boost::replace_all(content, "{#MEM}", to_string(round(ramusagelmt)));
		boost::replace_all(content, "{#CPU}", to_string(round(cpuusagelmt)));
		boost::replace_all(content, "{#PROCESS}", to_string(processusagelmt));
		boost::replace_all(content, "{#MEM_USGE}", to_string(round(dramusage)));
		boost::replace_all(content, "{#CPU_USGE}", to_string(round(dcpuusage)));
		boost::replace_all(content, "{#PROCESS_USGE}", to_string(dprocessusage));
		message.addPart("", new StringPartSource(content, "text/html; charset=utf-8"), MailMessage::CONTENT_INLINE, MailMessage::ENCODING_8BIT);
		SecureSMTPClientSession session(mailhost, 25);
		session.login();
		session.startTLS(pContext);
		if (!this->username.empty())
		{
			session.login(SMTPClientSession::AUTH_LOGIN, this->username, this->password);
		}
		session.sendMessage(message);
		session.close();
	}
	catch (const std::exception&ex)
	{
		cout << "Exception is SendMail " << ex.what() << endl;
		return -1;
	}
	return 0;


}
Utils::~Utils()
{
}
