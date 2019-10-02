#ifndef ITEST_H
#define ITEST_H
///////////////////////////////////////////////////////////////////////
// ITest.h - facilities for test harness                             //
// ver 1.1                                                           //
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
* This package provides two interfaces as protocol
* - ITest   Test interfaces for all tests implement
* - ITests  As a container, collect all points of 
*           Itest objects as protocol for Loader
*
* Required Files:
* ---------------
*   ITest.h
*
* Maintenance History:
* --------------------
* ver 1.2 : 10 Oct 2018
* - implement logger pointer into the interface, Itest for
* ver 1.1 : 07 Oct 2018
* - implement ITests which is an interface for Loader
* - setting DLL type proving export and import
* ver 1.0 : 13 Sep 2018
* - first release
* - taken from Utilities/TestUtilitiesDeme package
*
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

// export interfaces functions for Loader to use
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif


#include <string>
#include <vector>
#include "../SingletonLogger/ISingletonLogger.h"


// define ITest, there are virtual test() and info of test
using ILog = Utilities::ILogger<0, Utilities::Lock>;

class ITest
{
public:
	virtual DLL_DECL bool test() = 0;
	virtual DLL_DECL std::string name() = 0;
	virtual DLL_DECL std::string author() = 0;
	//virtual DLL_DECL ~ITest() {}

	virtual DLL_DECL void acceptlogger(ILog* test_logger) = 0;
};

//define ITests as a container of all ITest pointers 

class ITests
{
public:
	virtual DLL_DECL std::vector<ITest*> tests() = 0;
};

// different TestDriver DLLs will contain different implementations of get_ITests()
// however, the interface is still the same which allows Loader to handle all of
// the different TestDrivers the same way and extract their tests without knowing
// ANY test-specific information.

extern "C"
{
	DLL_DECL ITests* get_ITests();
}

#endif


