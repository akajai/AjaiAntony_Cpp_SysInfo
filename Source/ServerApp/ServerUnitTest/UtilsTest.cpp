#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ServerUnitTest
{
	Utils* utils;
	TEST_CLASS(ServerUnitTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialize)
		{
			utils = new Utils();
		}

		TEST_METHOD(LoadXML_ValidFile)
		{

			Assert::AreEqual(0, utils->loadXML("..\\..\\config.xml"));
			// TODO: Your test code here
		}

		TEST_METHOD(LoadXML_ValidFile_Clientcount)
		{
			int clientcount = utils->getClientList().size();

			Assert::AreEqual(4, clientcount);
			// TODO: Your test code here
		}
		
		TEST_METHOD(DBInit)
		{
			int clientcount = utils->SQLInit();

			Assert::AreEqual(0, clientcount);

			// TODO: Your test code here
		}
		TEST_METHOD(DBInsert1)
		{
			int ret = utils->LogDB("PC", 3, 4, 5);

			Assert::AreEqual(0, ret);

			// TODO: Your test code here
		}
		TEST_METHOD(DBInsert2)
		{
			int ret = utils->LogDB("PC2", 35.5, 57.66, 500);

			Assert::AreEqual(0, ret);

			// TODO: Your test code here
		}
		TEST_METHOD(RuleEngine1)
		{
			utils->loadXML("..\\..\\config.xml");
			utils->LoadMailTemplate("..\\..\\alert_template.html");

			Assert::AreEqual(0, utils->RuleEngine("PC01", 10, 10, 10));

			// TODO: Your test code here
		}
		TEST_METHOD(RuleEngine2)
		{
			utils->loadXML("..\\..\\config.xml");
			utils->LoadMailTemplate("..\\..\\alert_template.html");

			Assert::AreEqual(0, utils->RuleEngine("PC2", 20, 20, 20));

			// TODO: Your test code here
		}
		TEST_METHOD(RuleEngine3)
		{
			utils->loadXML("..\\..\\config.xml");
			utils->LoadMailTemplate("..\\..\\alert_template.html");

			int ret = utils->RuleEngine("PC3", 30, 30, 30);

			Assert::AreEqual(0, ret);

			// TODO: Your test code here
		}
		TEST_METHOD(RuleEngine4)
		{
			utils->loadXML("..\\..\\config.xml");
			utils->LoadMailTemplate("..\\..\\alert_template.html");
			int ret = utils->RuleEngine("PC4", 40, 40, 40);

			Assert::AreEqual(0, ret);

			// TODO: Your test code here
		}
		
		TEST_METHOD(LoadEmailTemplate_InValidFile)
		{
			Assert::AreEqual(-1, utils->LoadMailTemplate("..\\..\\alert_template1.html"));

			// TODO: Your test code here
		}
		TEST_METHOD(LoadEmailTemplate_ValidFile)
		{
			Assert::AreEqual(0, utils->LoadMailTemplate("..\\..\\alert_template.html"));

			// TODO: Your test code here
		}
		
		TEST_METHOD(SendMail1)
		{
			utils->loadXML("..\\..\\config.xml");
			utils->LoadMailTemplate("..\\..\\alert_template.html");
			int ret = utils->SendMail("PC_SendMail", "ajaiantonykolarikal@yahoo.com", 45, 34, 56, 34, 56, 78);

			Assert::AreEqual(0, ret);

			// TODO: Your test code here
		}

		TEST_METHOD(LoadXML_InValidFile)
		{

			Assert::AreEqual(-1, utils->loadXML("..\\..\\config1.xml"));
			// TODO: Your test code here
		}
		TEST_METHOD(LoadXML_InValidFile_Clientcount)
		{
			int clientcount = utils->getClientList().size();
			if (clientcount == 0) {
				Assert::AreEqual(0, clientcount);
			}
			// TODO: Your test code here
		}
		TEST_METHOD(DBInsert_Disconneted)
		{
			utils->SQLFree();
			int ret = utils->LogDB("PC2", 35.5, 57.66, 500);
			Assert::AreEqual(-1, ret);
			// TODO: Your test code here
		}
		
		TEST_CLASS_CLEANUP(CleanUp) {
			free(utils);
		}
	};
}