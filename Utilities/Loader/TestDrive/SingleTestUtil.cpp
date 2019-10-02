////////////////////////////////////////////////////////////////////////////
// SingleTestUtil.cpp : single test Test Driver                           //
// ver 1.0                                                                //
//                                                                        //
// Application : Project #2                                               //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Junhao Chen                                              //
// Original    : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This is a test driver package. It use Itest and Itests as 
*  a protocol defined in ITest.h package. For Loader, it doesn't care
*  if the TestDriver contains one or many Test Classes as it will attempt
*  to get the collection of ITest*. It will help for loader, executor and
*  executive in test.
*
*  Required files:
* =================
*  ITests.h SingleTestUtil.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - first release
*/

#define IN_DLL
#define ARG_SIZE 256  // for the manufactured cmd args

#include "../../ITest/ITest.h"
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>

//using namespace Utilities;
using namespace std;

using ILogger = Utilities::ILogger<0, Utilities::Lock>;

//////////////////////////////////////////////////////////////////////////////
// This TestPasses class for test of executor and executive
//template<int Category, template<int Category> class Locker>
class TestPasses : public ITest
{
public:
	DLL_DECL bool test() 
	{
		if (testlogger_ == nullptr)
		{
			cout << "\n TestPasses prints by cout." << endl;
		}
		else 
		{
			std::string fileSpec = "..\\Logtestdll.txt";
			std::ofstream outfileStrm(fileSpec, std::ios::out);
			if (outfileStrm.good())
				testlogger_->addStream(&outfileStrm);  // adding pointer to std::ofstream instance
			testlogger_->setAuthor("Junhao Chen");
			testlogger_->write("\n TestPasses prints by testlogger_."); // write contents at ofstreama snd ostream
			testlogger_->write("\n This is a demo dll for single tast \n"); // write contents at ofstreama snd ostream
			testlogger_->writeTail();
			testlogger_->removeStream(&outfileStrm);
		}
		return true; 
	}
	DLL_DECL std::string name()
	{
		std::string tname("TestPasses");
		return tname;
	}
	DLL_DECL std::string author()
	{
		std::string tauthor("Junhao Chen");
		return tauthor;
	}

	//template<int Category, template<int Category> class Locker>
	DLL_DECL void acceptlogger(ILogger* test_logger)
	{
		testlogger_ = test_logger;
	}

	//DLL_DECL ~TestPasses() { std::cout << "\n  -- deleting TestPasses"; }


private:
	//template<int Category, template<int Category> class Locker>
	ILogger* testlogger_ = nullptr;

};

//////////////////////////////////////////////////////////////////////////////
// test collection 

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new TestPasses);

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

