////////////////////////////////////////////////////////////////////
// Demo_Testreqs.cpp : This is a TestDrive DLL to demonstrate all //
//                     testrequests for Project#4                 //
// ver 1.0                                                        //
// Author      : Junhao CHen                                      //
////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  Demo_Testreqs provides 6 tests each test should demonstrate part 
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

class TestReq1 : public ITest
{
	/*
	* Test Description:
	*   Demonstrates use of C++
	* Test Procedure:
	*   Displays C++ files in project directory
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 1 - Use C++ \n";
		testlogger_->write(msg);
		msg = "------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by viewing files in project directory\n");
		Path path = "../Mother_server";
		Patterns pats = { "*.h", "*.cpp" };
		return Utilities::showDirContents(path, "", pats);
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq1");
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

class TestReq2 : public ITest
{
	/*
	* Test Description:
	*   Demonstrates use of WPF 
	* Test Procedure:
	* - Displays C# files in project directory
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 2 - Use Windows Presentation Foundation \n";
		testlogger_->write(msg);
		msg = "------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by viewing files in project directory\n");
		Path path = "../PrototypeGui-OOD-Pr4";
		Patterns pats = { "*.xaml", "*.cs" };
		return Utilities::showDirContents(path, "", pats);
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq 2");
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


class TestReq3a : public ITest
{
	/*
    *  Test Description:
    *    Demonstrates assemble working parts from Projects #1, #2, #3
	*    3a. show including packages in Tester.cpp which invokes files created by project #1.
	*    3b. show files in TestHarnessChild which are created in project #2.
	*    3c. show files in Mother_server which are created in project #3.
	*    And Clients need not wait for a reply for a TestRequest from the 
	*    TestHarness before sending additional TestRequests. You can play in
	*    GUI TestRequest field.
	*   
    *  Test Procedure:
	*  - Show content of including files in Tester.cpp
    */
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 3a - assemble parts from project #1. \n";
		testlogger_->write(msg);
		msg = "------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by show including packages in Tester.cpp which invokes files created by project #1.\n");
		Path path = "../TestHarnessChild/Tester.cpp";
		
		return Utilities::showFileLines(path, 6, 15);

	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq3a");
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

class TestReq3b : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates assemble working parts from Projects #1, #2, #3
	*    3a. show including packages in Tester.cpp which invokes files created by project #1.
	*    3b. show files in TestHarnessChild which are created in project #2.
	*    3c. show files in Mother_server which are created in project #3.
	*    And Clients need not wait for a reply for a TestRequest from the 
	*    TestHarness before sending additional TestRequests. You can play in
	*    GUI TestRequest field.
	*   
    *  Test Procedure:
	*  - Show files in the TestHarnessChild package created by project #2.
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 3b - assemble parts from project #2.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by showing files in the TestHarnessChild package created by project #2\n");
		Path path = "../TestHarnessChild";
		Patterns pats = { "*.h", "*.cpp" };
		return Utilities::showDirContents(path, "", pats);
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq3b");
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

class TestReq3c : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates assemble working parts from Projects #1, #2, #3
	*    3a. show including packages in Tester.cpp which invokes files created by project #1.
	*    3b. show files in TestHarnessChild which are created in project #2.
	*    3c. show files in Mother_server which are created in project #3.
	*    And Clients need not wait for a reply for a TestRequest from the 
	*    TestHarness before sending additional TestRequests. You can play in
	*    GUI TestRequest field.
	*   
    *  Test Procedure:
	*  - Show files in the Mother_server package created by project #3.
	*  - Note: GUI clients can continues to send testrequest without waiting.
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 3c - assemble parts from project #3.\n";
		testlogger_->write(msg);
		msg = "------------------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by showing files in the Mother_server package created by project #3.\n");
		testlogger_->write("Note: GUI clients can continue to send testrequests without any waiting. Playing in GUI client.\n");
		Path path = "../Mother_server";
		Patterns pats = { "*.h", "*.cpp" };
		return Utilities::showDirContents(path, "", pats);

	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq3c");
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

class TestReq4a : public ITest
{
	/*
	*  Test Description:
	*    4a. Proving GRaphical User Interface (GUI) for Client,
	*    4b. Supporting selecting test libraries and send for testing,
	*    4c. Supporting displaying test Result.
	*
	*  Test Procedure:
	*  - display the begin of codes in MainWindow.xaml
	*  - Please, play with GUI client, when running. 
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 4a - Proving GRaphical User Interface (GUI) for Client.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by displaying the begin of codes in MainWindow.xaml \n");
		testlogger_->write("Please, play with GUI client, when running.  \n");
		Path path = "../PrototypeGui-OOD-Pr4/MainWindow.xaml";
		return Utilities::showFileLines(path, 1, 15);

	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq4a");
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

class TestReq4b : public ITest
{
	/*
	*  Test Description:
	*    4a. Proving GRaphical User Interface (GUI) for Client,
	*    4b. Supporting selecting test libraries and send for testing,
	*    4c. Supporting displaying test Result.
	*
	*  Test Procedure:
	*  - display codes for selecting and sending in MainWindow.xaml.cs
	*  - Please, play with GUI client, when running.
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 4b - Supporting selecting test libraries and send for testing.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by displaying codes of selecting test dll and sending testrequest.\n");
		testlogger_->write("Note: Selecting by clicking items in Files listbox and triggering Files_SelectionChanged(). \n");
		testlogger_->write("Sending files is triggered by click submit button on SelectionWindow, executing SendMessage(). \n");
		testlogger_->write("it Invokes CreateTestrequest(), CopyandSendFiles and SendingFiles() to help.\n");
		testlogger_->write("Display Files_SelectionChanged() and SendMessage().\n");
		Path path = "../PrototypeGui-OOD-Pr4/MainWindow.xaml.cs";
		bool fs = Utilities::showFileLines(path, 145, 172);
		bool sm = Utilities::showFileLines(path, 231, 258);
		return (fs && sm);

	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq4b");
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

class TestReq4c : public ITest
{
	/*
	*  Test Description:
	*    4a. Proving GRaphical User Interface (GUI) for Client,
	*    4b. Supporting selecting test libraries and send for testing,
	*    4c. Supporting displaying test Result.
	*
	*  Test Procedure:
	*  - display codes for displaying in MainWindow.xaml.cs
	*  - Please, play with GUI client, when running.
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 4c - Supporting displaying test Result.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by displaying codes of showing testrequest message.\n");
		testlogger_->write("Note: ProcessMessages() will continuely fetch message and trigger to show test result by matching command.\n");
		Path path = "../PrototypeGui-OOD-Pr4/MainWindow.xaml.cs";
		return Utilities::showFileLines(path, 304, 326);
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq4c");
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
class TestReq5 : public ITest
{
	/*
	*  Test Description:
	*    Proving message design, it has the same class and have
	*    a specified set of attributes for tasks
	*
	*  Test Procedure:
	*  - display constructing massage code
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 5 - Proving message design.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Demonstrate by displaying constructing massage code for sending files and testrequest. \n");
		Path path = "../PrototypeGui-OOD-Pr4/MainWindow.xaml.cs";
		bool sf = Utilities::showFileLines(path, 224, 229);
		bool sm = Utilities::showFileLines(path, 244, 257);
		return (sf && sm);
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq5");
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

class TestReq6 : public ITest
{
	/*
	*  Test Description:
	*    Proving sending TestRequest messages from Client to TestHarness Server.
	*    recieve results.
	*
	*  Test Procedure:
	*  - display source code to wrap TestReqest into message for sending
	*  - display recieve code 
	*/
public:
	DLL_DECL bool test()
	{
		Message msg = "\n  TestReq 6 - Proving sending TestRequest messages from Client to TestHarness Server.\n";
		testlogger_->write(msg);
		msg = "---------------------------------------------------------------------------------------\n";
		testlogger_->write(msg);
		testlogger_->write("Please Check SendMessage() which shows at Test 4b.\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestReq6");
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
	tests_vec.push_back(new TestReq1);
	tests_vec.push_back(new TestReq2);
	tests_vec.push_back(new TestReq3a);
	tests_vec.push_back(new TestReq3b);
	tests_vec.push_back(new TestReq3c);
	tests_vec.push_back(new TestReq4a);
	tests_vec.push_back(new TestReq4b);
	tests_vec.push_back(new TestReq4c);
	tests_vec.push_back(new TestReq5);
	tests_vec.push_back(new TestReq6);
	
	
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