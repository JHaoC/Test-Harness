#pragma once
///////////////////////////////////////////////////////////////////////
// Executor.h - facilities for single-user test harness               //
// ver 1.3                                                           //
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
* - Executor    Executes single test in the context of a try-catch block
*
* Required Files:
* ---------------
* ITest.h, SingletonLogger.h
* Excutor.h, Excutor.cpp
*
* Maintenance History:
* --------------------
* ver 1.3 : 5 Nov 2018
* - modify for new Untilitie Env
* - calling getinstance for log
* ver 1.2 : 6 Oct 2018
* - putting logger pointer to show contents
* ver 1.1 : 20 Sep 2018
* - modifity execute to show authors 
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
#include <iostream>
#include <string>
#include "../ITest/ITest.h"
#include "../SingletonLogger/SingletonLogger.h"


namespace Utilities 
{

	/////////////////////////////////////////////////////////////////////
	// TestExecutor class
	// - supports execution of callable objects for testing in the
	//   context of a try-catch block.
	
	class Executor
	{
	public:
		bool execute(std::shared_ptr<ITest> pTest, const std::string& name);
	private:
		void check(bool result);
	};
	//----< execute tests in the context of a try-catch block >----------

	inline bool Executor::execute(std::shared_ptr<ITest> pTest, const std::string& name)
	{
		ILog* pLog = Logger<0, Utilities::Lock>::getInstance();

		bool result = false;
		try
		{
			result = pTest->test();  // execute test which may throw an exception
			check(result);
			std::string msg = " -- \"" + name + "\"\n";
			pLog->write(msg);
		}
		catch (std::exception& ex)
		{
			check(false);
			std::string msg = " -- \"" + name + "\"\n";
			pLog->write(msg);
			pLog->write(ex.what());
		}
		return result;
	}
	//----< display test results >---------------------------------------

	inline void Executor::check(bool result)
	{
		ILog* pLog = Logger<0, Utilities::Lock>::getInstance();
		if (result)
			pLog->write("\n  passed");
			//std::cout << "\n passed" << std::endl;
		else
			pLog->write("\n  failed");
			//std::cout << "\n false" << std::endl;
	}

}