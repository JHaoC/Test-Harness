#pragma once
///////////////////////////////////////////////////////////////////////
// Executive.h - facilities for single-user test harness               //
// ver 1.4                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Junhao Chen                                          //
//              juchen@syr.edu                                       //
// Original:    Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a class:
* - Executive   Uses Executor to run a sequence of tests
*
* Required Files:
* ---------------
*  ITest.h, Executor.h
*  Executive.h, Exective.cpp
*   
*
* Maintenance History:
* --------------------
* ver 1.5 : 5 Nov 2018
* - remove loading()
* ver 1.4 : 11 Oct 2018
* - modify dotest() and loading()
* ver 1.3 : 10 Oct 2018
* - implement loader
* - implement loading() 
* ver 1.2 : 06 Oct 2018
* - implement to SingletonLogger and remove logger
* - retry cpp for test
* ver 1.1 : 20 Sep 2018
* - implement logger
* ver 1.0 : 13 Sep 2018
* - first release
* - taken from Utilities/TestUtilities package
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <memory>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include "../ITest/ITest.h"
#include "../Executor/Executor.h"
#include "../SingletonLogger/SingletonLogger.h"

namespace Utilities
{
	using Path = std::string;
	using Message = std::string;
	using Line = size_t;
	using File = std::string;
	using Files = std::vector<File>;
	using Filename = std::string;

	///////////////////////////////////////////////////////////////////////
	// TestExecutive class
	// - executes a sequence of tests with the help of TestExecutor

	class Executive
	{
	public:
		using TestItem = struct {
			std::shared_ptr<ITest> pTest;
			std::string testName;
		};
		using Tests = std::vector<TestItem>;
		bool doTests();
		void registerTest(std::shared_ptr<ITest> pTest, const std::string& testName);
		void registerTest(const TestItem& ts);
		void clearTests();

	private:
		Tests tests_;  // collection of test items
	};
	//----< store test in test collection for later execution >--------

	inline void Executive::registerTest(std::shared_ptr<ITest> pTest, const std::string& testName)
	{
		TestItem ts{ pTest, testName };
		tests_.push_back(ts);
		// push_back copies the test item
		// - that makes a new shared_ptr sharing the same test as the original
		// - it also makes a copy of the test name string
	}

	inline void Executive::registerTest(const TestItem& ts)
	{
		tests_.push_back(ts);
		// push_back copies the test item
		// - that makes a new shared_ptr sharing the same test as the original
		// - it also makes a copy of the test name string
	}

	inline void Executive::clearTests()
	{
		tests_.clear();
	}

	//----< execute each stored test >---------------------------------

	inline bool Executive::doTests()
	{
		Executor tester;
		ILog* pLog = Logger<0, Utilities::Lock>::getInstance();
		
		pLog->setTerminator("");
		bool result = true;
		for (auto item : tests_)
		{
			bool tResult = tester.execute(item.pTest, item.testName);
			if (tResult == false)
				result = false;
		}
		return result;
	}
	
}




	