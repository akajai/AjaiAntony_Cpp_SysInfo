// ServerApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Utils.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using namespace std;

void handle_get(http_request request)
{
	try
	{
		cout << "handle GET" << endl;
		map<utility::string_t, utility::string_t> http_get_vars = uri::split_query(request.request_uri().query());
		string strkey = utility::conversions::to_utf8string(http_get_vars[L"clientkey"]);
		string strramusage = utility::conversions::to_utf8string(http_get_vars[L"ramusage"]);\
		string strcpuusage = utility::conversions::to_utf8string(http_get_vars[L"cpuusage"]);
		string strprocessusage = utility::conversions::to_utf8string(http_get_vars[L"processusage"]);
	
		double dramusage = atof(strramusage.c_str());
		int dprocessusage = atoi(strprocessusage.c_str());
		float dcpuusage = atof(strcpuusage.c_str());

		cout << "clientkey : " << strkey << " Ramusage: " << dramusage << " CPUusage : " << strcpuusage << " Processusage : " << strprocessusage << endl;
		Utils utils;
		if (-1== utils.loadXML("config.xml")) {
			cout << " Error in loadXML " << endl;
		}
		if (-1== utils.LoadMailTemplate("alert_template.html")) {
			cout << "Error in LoadMailTemplate " << endl;
		}
		if (-1 == utils.SQLInit()) {
			cout << "Error in SQLInit " << endl;
		}
		if (-1 == utils.LogDB(strkey, dramusage, dcpuusage, dprocessusage)) {
			cout << " Error in DB operation " << endl;
		}
		if (-1 == utils.SQLFree()) {
			cout << " Error in SQLFree" << endl;
		}
		if (-1 == utils.RuleEngine(strkey, dramusage, dcpuusage, dprocessusage)) {
			cout << "Error in Rule Engine " << endl;
		}
		request.reply(status_codes::OK);
	}
	catch (const std::exception& e)
	{
		cout << "Exception in handle_get " << e.what() << endl;
		request.reply(status_codes::BadRequest);
	}

}
int main()
{
	//http://localhost:8080/Update?clientkey=clientkey&ramusage=50&cpuusage=25&processusage=60
	http_listener listener(L"http://localhost:8080/Update");
	cout << "http://localhost:8080/Update Listening -----" << endl;
	listener.support(methods::GET, handle_get);
	try
	{
		listener
			.open()
			.then([&listener]() {cout << " http://localhost:8080/Update  starting to listen" << endl; })
			.wait();
		while (true);
	}
	catch (exception const & e)
	{
		cout << "http://localhost:8080/Update Listening failed \n";
		cout << e.what() << endl;
	}

	return 0;

}

