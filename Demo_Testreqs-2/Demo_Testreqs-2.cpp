////////////////////////////////////////////////////////////////////
// Demo_Testreqs-2.cpp : This is a TestDrive DLL to demonstrate   //
//                     testrequests for Project#4                 //
// ver 1.0                                                        //
// Author      : Junhao CHen                                      //
////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  Demo_Testreqs provides 4 tests each test should demonstrate part 
*  of a requirement. This package as dll in the TestDll-clt folder. 
*  It will send to the server side by client and do test at childtester.
*  Thit result of logging return to GUI client 
*
*  Required files:
* =================
*  ITests.h
*  StringUtilities.h
*  Demo_Testreqs.cpp
*  FileUtilities.h
*  
*
*  Maintainence History:
* =======================
*  ver 1.0 : 03 Dec 2018
*  - first release
*/

#define IN_DLL
#define ARG_SIZE 256

#include "../Utilities/ITest/ITest.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include <iostream>
#include <string>
#include <string.h>

using ILogger = Utilities::ILogger<0, Utilities::Lock>;
using Message = std::string;
using Pattern = std::string;
using Patterns = std::vector<Pattern>;
using Path = std::string;

class TestReq7 : public ITest
{
	/*
	*  Test Description:
	*    Supporting each testRequest executing in its own process pool child process
	*  Test Procedure:
	*  - create a testrequest including Demo_Testreques_1 and Demo_Testreques_2 by test().
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 7 - Supporting each testRequest executing in its own process pool child process.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by creating a TestRequest including Demo_Testreques_1 and Demo_Testreques_2. \n");
		testlogger_->write("Note: Once creating a testRequest, all of dlls are bonded and executed together \n at one and only one child tester process which assigned by server-mother\n");
		testlogger_->write("The testrequest is created by Test1(). You can see the Demo_Testreques_1 and Demo_Testreques_2 running at the same child process.");
		testlogger_->write("Here also show Test() code");
		Path path = "../PrototypeGui-OOD-Pr4/MainWindow.xaml.cs";
		return Utilities::showFileLines(path, 346, 357);

	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq7");
		return tname;
	}
	DLL_DECL std::string author()
	{
		std::string tauthor("Junhao Chen");
		return tauthor;
	}
	DLL_DECL void acceptlogger(ILogger* test_logger)
	{
		testlogger_ = test_logger;
	}

private:
	ILogger* testlogger_ = nullptr;

};

class TestReq8 : public ITest
{
	/*
	* Test Description:
	*   Demonstrates sending test results from child process to the client that made the request.
	* Test Procedure:
	*   Displays parts of getTestrequest() which is constructing massage code in Tester.cpp.
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq8 - sending test results from child process to the client that made the request \n";
		testlogger_->write(msg);
		msg = "------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by viewing parts of getTestrequest() which is constructing massage code in Tester.cpp.\n");
		testlogger_->write("You can see test results return back at GUI clients which made test requests");
		Path path = "../TestHarnessChild/Tester.cpp";
		return Utilities::showFileLines(path, 108, 116);
		
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq8");
		return tname;
	}
	DLL_DECL std::string author()
	{
		std::string tauthor("Junhao Chen");
		return tauthor;
	}
	DLL_DECL void acceptlogger(ILogger* test_logger)
	{
		testlogger_ = test_logger;
	}

private:
	ILogger* testlogger_ = nullptr;

};


class TestReq9 : public ITest
{
	/*
    *  Test Description:
    *    Demonstrates correct test operations for two or more concurrent clients.
	*   
    *  Test Procedure:
	*  - Run two clients at same time. one of is a demo client which will run Test1().  
	*    This client is only working for demo.
    */
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 9 - Demonstrates correct test operations for two or more concurrent clients. \n";
		testlogger_->write(msg);
		msg = "------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by running two clients at same time. one of is a demo client which will run Test1().\n");
		testlogger_->write("This demo client only existed for demo. You can shut down after demo. \n");
		testlogger_->write("Notes: GUI client needs three parameters to exe. It will run version of demo when the third parameter is Demo\n");
		return true;

	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq9");
		return tname;
	}
	DLL_DECL std::string author()
	{
		std::string tauthor("Junhao Chen");
		return tauthor;
	}
	DLL_DECL void acceptlogger(ILogger* test_logger)
	{
		testlogger_ = test_logger;
	}

private:
	ILogger* testlogger_ = nullptr;

};

class TestReq10 : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates include an automated unit test suite that demonstrates
	*    your Clients and TestHarness satisfy all of the functional requirements
	*   
    *  Test Procedure:
	*  - automated triggered Test() when running Demo version 
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 10 - automated triggered.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by automating triggered Test() when running Demo version  \n");
		return true;
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq10");
		return tname;
	}
	DLL_DECL std::string author()
	{
		std::string tauthor("Junhao Chen");
		return tauthor;
	}
	DLL_DECL void acceptlogger(ILogger* test_logger)
	{
		testlogger_ = test_logger;
	}
private:
	ILogger* testlogger_ = nullptr;

};


//////////////////////////////////////////////////////////////////////////////
// test collection 

class TestCollection : public ITests 
{
	DLL_DECL std::vector<ITest*> tests();
};

DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new TestReq7);
	tests_vec.push_back(new TestReq8);
	tests_vec.push_back(new TestReq9);
	tests_vec.push_back(new TestReq10);
	
	return tests_vec;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// this section is where each Test Driver DLL completely differs from other
// test drivers. Although the same name can be used in all TestDrivers, the 
// actual instance of TestCollection is different in the way it returns 
// different collection of ITest*.

DLL_DECL ITests* get_ITests()
{
	return new TestCollection;
}