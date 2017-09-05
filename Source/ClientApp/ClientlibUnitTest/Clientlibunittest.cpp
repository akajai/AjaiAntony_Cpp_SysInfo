#include "stdafx.h"
#include "CppUnitTest.h"
#include "ClientUtils.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientlibUnitTest
{		
	ClientUtils *clientlib;
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialize)
		{
			 clientlib = new ClientUtils();
			 Assert::IsNotNull(clientlib);
		}
		TEST_METHOD(LogData)

		{
			Assert::AreEqual(0, clientlib->LogInfo());
			// TODO: Your test code here
		}
		TEST_METHOD(InitHttpUrl)

		{
			Assert::AreEqual(0, clientlib->InitHttpUrl("localhost"));
			// TODO: Your test code here
		}
		TEST_METHOD(GetHttpUrllocalhost)

		{
			clientlib->InitHttpUrl("localhost");
			Assert::AreEqual("http://localhost:8080/Update", clientlib->getHttpUrl().c_str(),true);
			// TODO: Your test code here
		}
		TEST_METHOD(GetHttpUrlIploopback)

		{
			clientlib->InitHttpUrl("127.0.0.1");
			Assert::AreEqual("http://127.0.0.1:8080/Update", clientlib->getHttpUrl().c_str(), true);
			// TODO: Your test code here
		}
		TEST_METHOD(GetHttpUrlIp)

		{
			clientlib->InitHttpUrl("192.168.125.1");
			Assert::AreEqual("http://192.168.125.1:8080/Update", clientlib->getHttpUrl().c_str(), true);
			// TODO: Your test code here
		}
		TEST_METHOD(GetComputerName_ClientKey)

		{
			int retcode = clientlib->getHostName().length();
			Assert::AreNotEqual(0, retcode);
			// TODO: Your test code here
		}
		TEST_METHOD(GetMemoryUsage)

		{
			Assert::AreNotEqual(-1.0, clientlib->GetMemoryUsage());
			// TODO: Your test code here
		}
		TEST_METHOD(GetProcessCount)

		{
			Assert::AreNotEqual(-1.0, clientlib->GetProcessCount());
			// TODO: Your test code here
		}
		TEST_METHOD(GetCpuUsage)

		{
			
			Assert::AreNotEqual(-1.0, (double)clientlib->GetCpuUsage());
			// TODO: Your test code here
		}


	};
}